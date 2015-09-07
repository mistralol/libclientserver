
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
	m_mutex.WakeUp(); //Tell thread to wakeup and exit
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
	m_mutex.WakeUp();
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
	m_mutex.WakeUp();
}

void Timers::Run()
{
	m_mutex.Lock(); //We really only unlock when we are asleep
	while(m_run == true)
	{
		struct timespec ts;
		if (clock_gettime(CLOCK_MONOTONIC, &ts) != 0)
			abort();

		time_t curtime = ts.tv_sec;

		std::multimap<time_t, ITimer *>::iterator it = m_timers.begin();
		if (it == m_timers.end())
		{
			m_mutex.Wait(); //Wait forever on empty list until told otherwise
			continue; //Restart
		}

		if (curtime < it->first)
		{
			struct timespec delay;
			delay.tv_sec = it->first - curtime;
			delay.tv_nsec = 0;
			m_mutex.Wait(&delay);
		}

		it = m_timers.begin();
		if (it == m_timers.end())
			continue; //All timers we removed while we were sleeping
		
		if (curtime >= it->first)
		{	
			ITimer *timer = it->second;
			m_timers.erase(it);
			timer->TimerExpired(this, timer);
			continue;
		}
	}
	m_mutex.Unlock();
}

