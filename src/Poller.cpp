
#include <libclientserver.h>

Poller::Poller()
{
	m_loop = true;
	m_controlfd = eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
	if (m_controlfd < 0)
		abort();
	m_modified = false;
	m_err_ebadf = 0;
	Thread::Start();
}

Poller::~Poller()
{
	m_loop = false;
	WakeUp();
	Thread::Stop();

	do
	{
		ScopedLock lock = ScopedLock(&m_mutex);
	restart:
		m_modified = false;
		for(std::map<int, IPollable *>::iterator it = m_map.begin(); it != m_map.end(); it++)
		{
			it->second->DoClose(this);
			if (m_modified)
				goto restart;
		}
	} while(0);

	if (m_map.size() > 0)
	{
		abort(); //Tried to delete selector with items left in it
	}
	
	if (close(m_controlfd) < 0)
		abort();
}

void Poller::Add(IPollable *p)
{
	int fd = p->GetFD(this);
	if (fd < 0)
		abort(); //Invalid FD this is a bug
	if (m_map.find(fd) != m_map.end())
		abort(); //Duplicate FD?

	ScopedLock lock = ScopedLock(&m_mutex);

	m_modified = true;
	m_map[fd] = p;
	UpdateMap(fd);
	WakeUp();
}

void Poller::Update(IPollable *p)
{
	ScopedLock lock = ScopedLock(&m_mutex);
	m_modified = true;
	UpdateMap(p->GetFD(this));
	WakeUp();
}

void Poller::Remove(IPollable *p)
{
	int fd = p->GetFD(this);
	if (fd < 0)
		abort(); //Invalid FD this is a bug
	if (m_map.find(fd) == m_map.end())
		abort(); //No FD to remove

	ScopedLock lock = ScopedLock(&m_mutex);
	m_modified = true;
	m_map.erase(m_map.find(fd));

	std::map<int, struct timespec>::iterator it = m_timeout.find(fd);
	if (it != m_timeout.end())
		m_timeout.erase(it);

	WakeUp();
}

void Poller::WakeUp()
{
	uint64_t x = 1;

	ScopedLock lock = ScopedLock(&m_mutex);
	if (write(m_controlfd, &x, sizeof(x)) != sizeof(x))
	{
		abort();
	}
}

void Poller::UpdateMap(int fd)
{
	std::map<int, IPollable *>::iterator it = m_map.find(fd);
	if (it == m_map.end())
		abort();

	if (it->second->CanTimeout(this))
	{
		struct timespec ts, now;
		it->second->GetTimeout(this, &ts);

		if (ts.tv_sec == 0 && ts.tv_nsec == 0)
		{
			std::map<int, struct timespec>::iterator it2 = m_timeout.find(fd);
			if (it2 != m_timeout.end())
				m_timeout.erase(it2);
		}

		if (clock_gettime(CLOCK_MONOTONIC, &now) < 0)
			abort();

		ts.tv_sec += now.tv_sec;
		ts.tv_nsec += now.tv_nsec;

		if (ts.tv_nsec > 999999999)
		{
			ts.tv_sec++;
			ts.tv_nsec -= 1000000000;
		}

		m_timeout[it->second->GetFD(this)] = ts;
	}
}

void Poller::ReadControl()
{
	uint64_t x = 1;
	int size = 0;
	do {
		size = read(m_controlfd, &x, sizeof(x));
		if (size < 0)
		{
			switch(errno)
			{
				case EAGAIN:
					size = 0;
					break;
				default:
					abort();
					break;
			}
		}
	} while(size != 0);
}

void Poller::CalcTimeout(struct timespec *tv)
{
	struct timespec now;
	struct timespec lowest;

	lowest.tv_sec = LONG_MAX;
	lowest.tv_nsec = 0;

	tv->tv_sec = 0;
	tv->tv_nsec = 0;

	if (clock_gettime(CLOCK_MONOTONIC, &now) < 0)
		abort();
	std::map<int, struct timespec>::iterator it = m_timeout.begin();
	while(it != m_timeout.end())
	{
		if (Time::IsLess(&it->second, &lowest))
		{
			lowest.tv_sec = it->second.tv_sec;
			lowest.tv_nsec = it->second.tv_nsec;
		}
		it++;
	}

	Time::Sub(&lowest, &now, &lowest);
	if (lowest.tv_sec < 0)
		return;
	tv->tv_sec = lowest.tv_sec;
	tv->tv_nsec = lowest.tv_nsec;
}

void Poller::Run()
{
	struct pollfd *fds = NULL;
	size_t fds_size = 0;

	while(m_loop)
	{
		struct timespec timeout = {0, 0};
		//Rebuild Poll structure
		do
		{
			ScopedLock lock = ScopedLock(&m_mutex);
			if (fds_size != m_map.size() + 1)
			{
				fds_size = m_map.size() + 1;
				fds = (struct pollfd * ) realloc(fds, sizeof(*fds) * fds_size);
				if (fds == NULL)
					abort(); //malloc failure.
			}
			
			//First item is special case for control
			fds[0].fd = m_controlfd;
			fds[0].events = POLLIN;
			fds[0].revents = 0;

			int idx = 1;
			std::map<int, IPollable *>::iterator it = m_map.begin();
			while(it != m_map.end())
			{
				fds[idx].fd = it->first;
				fds[idx].events = 0;
				if (it->second->CanRead(this))
					fds[idx].events |= POLLIN;
				if (it->second->CanWrite(this))
					fds[idx].events |= (POLLOUT | POLLERR);
				fds[idx].revents = 0;
				it++;
				idx++;
			}
			CalcTimeout(&timeout);
		} while(0);

		int ret = ppoll(fds, fds_size, &timeout, NULL);
		if (ret < 0)
		{
			switch(errno)
			{
				case EINTR:
					continue;
					break;
				case EBADF: /* It is posisble to have a bad fd because the set could have changed after the unlock */
					m_err_ebadf++;
					continue;
					break;
				default:
					abort();
					break;
			}
		}

		do {
			ScopedLock lock = ScopedLock(&m_mutex);
			m_modified = false;

			for(size_t i=1;i<fds_size;i++)
			{
				bool work = false;
				std::map<int, IPollable *>::iterator it = m_map.find(fds[i].fd);
				if (it == m_map.end())
					continue; //Item must have been removed while we were in poll so skip it.

				if ( (fds[i].revents & POLLIN) )
				{
					it->second->DoRead(this);
					if (m_modified)
						goto skip_to_end;
					work = true;
				}

				if ( (fds[i].revents & POLLOUT) || (fds[i].revents & POLLERR))
				{
					it->second->DoWrite(this);
					if (m_modified)
						goto skip_to_end;
					work = true;
				}

				if (work)
				{
					if (it->second->CanTimeout(this))
					{
						struct timespec ts, now;
						it->second->GetTimeout(this, &ts);

						if (ts.tv_sec == 0 && ts.tv_nsec == 0)
						{
							std::map<int, struct timespec>::iterator it2 = m_timeout.find(fds[i].fd);
							if (it2 != m_timeout.end())
								m_timeout.erase(it2);
						}

						if (clock_gettime(CLOCK_MONOTONIC, &now) < 0)
							abort();

						ts.tv_sec += now.tv_sec;
						ts.tv_nsec += now.tv_nsec;

						if (ts.tv_nsec > 999999999)
						{
							ts.tv_sec++;
							ts.tv_nsec -= 1000000000;
						}
						m_timeout[it->second->GetFD(this)] = ts;
					}
					else
					{
						std::map<int, struct timespec>::iterator it2 = m_timeout.find(fds[i].fd);
						if (it2 != m_timeout.end())
							m_timeout.erase(it2);
					}
				}
			}

			struct timespec now = {0, 0};
			if (clock_gettime(CLOCK_MONOTONIC, &now) < 0)
				abort();

			for(std::map<int, struct timespec>::iterator it = m_timeout.begin(); it != m_timeout.end(); it++)
			{
				if (Time::IsLess(&it->second, &now))
				{
					m_map[it->first]->DoTimeout(this);
					if (m_modified)
						goto skip_to_end;
					UpdateMap(it->first);
				}
			}

		} while(0);
skip_to_end:
		ReadControl();
	}
}

