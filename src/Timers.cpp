
#include <libclientserver.h>

void Timers::Start()
{
	m_run = true;
	Thread::Start();
}

void Timers::Stop()
{
	if (m_timers.size() != 0)
		abort(); //We still have active timers

	m_run = false;
	Thread::Stop();
}

void Timers::Add(ITimer *timer)
{
	ScopedLock lock(&m_mutex);

	struct timespec ts;
	if (clock_gettime(CLOCK_MONOTONIC, &ts) != 0)
		abort();

	time_t curtime = ts.tv_sec + timer->GetDelay();
	m_timers.insert( std::pair<time_t, ITimer *>(curtime, timer) );
}

void Timers::Remove(ITimer *timer)
{
	ScopedLock lock(&m_mutex);
	std::multimap<time_t, ITimer *>::iterator it = m_timers.begin();
	while(it != m_timers.end())
	{
		if (it->second == timer)
		{
			m_timers.erase(it);
			return;
		}
		it++;
	}
}

void Timers::Run()
{
	while(m_run == true)
	{
		do {
			struct timespec ts;
			if (clock_gettime(CLOCK_MONOTONIC, &ts) != 0)
				abort();
			time_t curtime = ts.tv_sec;
			m_mutex.Lock();
			std::multimap<time_t, ITimer *>::iterator it = m_timers.begin();
			while(it != m_timers.end())
			{
				if (curtime >= it->first)
				{
					ITimer *timer = it->second;
					m_timers.erase(it);
					m_mutex.Unlock();
					timer->TimerExpired(this, timer);
					m_mutex.Lock();
					it = m_timers.begin();
					continue;
				}
				break;
			}
			m_mutex.Unlock();
		} while(0);
		sleep(1);
	}
}

