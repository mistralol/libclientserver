
#include <libclientserver.h>

Selector::Selector()
{
	m_loop = true;
	m_controlfd = eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
	if (m_controlfd < 0)
		abort();
	FD_ZERO(&m_freads);
	FD_ZERO(&m_fwrites);
	FD_ZERO(&m_fexcept);
	FD_SET(m_controlfd, &m_freads);
	FindHighestFD(); /* Init for m_maxfs */
	m_modified = false;
	m_err_ebadf = 0;
	Thread::Start();
}

Selector::~Selector()
{
	m_loop = false;
	WakeUp();
	Thread::Stop();

	do
	{
		ScopedLock lock = ScopedLock(&m_mutex);
	restart:
		m_modified = false;
		for(std::map<int, ISelectable *>::iterator it = m_map.begin(); it != m_map.end(); it++)
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

void Selector::Add(ISelectable *p)
{
	int fd = p->GetFD(this);
	if (fd < 0 || fd >= FD_SETSIZE)
		abort(); //Invalid FD this is a bug
	if (m_map.find(fd) != m_map.end())
		abort(); //Duplicate FD?

	ScopedLock lock = ScopedLock(&m_mutex);

	m_modified = true;
	m_map[fd] = p;
	UpdateMap(fd);
	FindHighestFD();
	WakeUp();
}

void Selector::Update(ISelectable *p)
{
	ScopedLock lock = ScopedLock(&m_mutex);
	m_modified = true;
	UpdateMap(p->GetFD(this));
	WakeUp();
}

void Selector::Remove(ISelectable *p)
{
	int fd = p->GetFD(this);
	if (fd < 0 || fd >= FD_SETSIZE)
		abort(); //Invalid FD this is a bug
	if (m_map.find(fd) == m_map.end())
		abort(); //No FD to remove

	ScopedLock lock = ScopedLock(&m_mutex);
	m_modified = true;
	FD_CLR(fd, &m_freads);
	FD_CLR(fd, &m_fwrites);
	FD_CLR(fd, &m_fexcept);
	m_map.erase(m_map.find(fd));

	std::map<int, struct timespec>::iterator it = m_timeout.find(fd);
	if (it != m_timeout.end())
		m_timeout.erase(it);

	FindHighestFD();
	WakeUp();
}

void Selector::FindHighestFD()
{
	ScopedLock lock = ScopedLock(&m_mutex);
	int high = m_controlfd;
	for(std::map<int, ISelectable *>::iterator it = m_map.begin(); it != m_map.end(); it++)
	{
		if (it->first > high)
			high = it->first;
	}
	m_maxfd = high;
}

void Selector::WakeUp()
{
	uint64_t x = 1;

	ScopedLock lock = ScopedLock(&m_mutex);
	if (write(m_controlfd, &x, sizeof(x)) != sizeof(x))
	{
		abort();
	}
}

void Selector::UpdateMap(int fd)
{
	std::map<int, ISelectable *>::iterator it = m_map.find(fd);
	if (it == m_map.end())
		abort();

	if (it->second->CanRead(this))
	{
		FD_SET(fd, &m_freads);
	}
	else
	{
		FD_CLR(fd, &m_freads);
	}

	if (it->second->CanWrite(this))
	{
		FD_SET(fd, &m_fwrites);
	}
	else
	{
		FD_CLR(fd, &m_fwrites);
	}

	if (it->second->CanExcept(this))
	{
		FD_SET(fd, &m_fexcept);
	}
	else
	{
		FD_CLR(fd, &m_fexcept);
	}

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
	else
	{
		std::map<int, struct timespec>::iterator it2 = m_timeout.find(fd);
		if (it2 != m_timeout.end())
			m_timeout.erase(it2);
	}
}

void Selector::ReadControl()
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

void Selector::CalcTimeout(struct timespec *tv)
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

void Selector::Run()
{
	while(m_loop)
	{
		struct timespec timeout;
		fd_set freads = m_freads;
		fd_set fwrites = m_fwrites;
		fd_set fexcept = m_fexcept;
		CalcTimeout(&timeout);
		int ret = pselect(m_maxfd + 1, &freads, &fwrites, &fexcept, &timeout, NULL);
		if (ret < 0)
		{
			switch(errno)
			{
				case EINTR:
					continue;
					break;
				case EBADF: /* It is possible to have a bad fd inside the set because it just changed after read took a copy of the set */
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
			//Check fd_set's
			for(std::map<int, ISelectable *>::iterator it = m_map.begin(); it != m_map.end(); it++)
			{
				int fd = it->first;
				bool work = false;
				if (FD_ISSET(fd, &freads))
				{
					it->second->DoRead(this);
					if (m_modified)
						goto skip_to_end;
					work = true;
				}
				if (FD_ISSET(fd, &fwrites))
				{
					it->second->DoWrite(this);
					if (m_modified)
						goto skip_to_end;
					work = true;
				}
				if (FD_ISSET(fd, &fexcept))
				{
					it->second->DoExcept(this);
					if (m_modified)
						goto skip_to_end;
					work = true;
				}

				if (work)
					UpdateMap(fd);
			}
					
			struct timespec now;
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

