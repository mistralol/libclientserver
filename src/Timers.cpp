
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

	struct timespec timeout;
	timer->GetDelay(&timeout);

	struct timespec expire;
	Time::Add(&ts, &timeout, &expire);

	unsigned long long curtime = Time::NanoSeconds(&expire);
	m_timers.insert( std::pair<unsigned long long, ITimer *>(curtime, timer) );
	m_mutex.WakeUp();
}

void Timers::Remove(ITimer *timer)
{
	ScopedLock lock(&m_mutex);
	std::multimap<unsigned long long, ITimer *>::iterator it = m_timers.begin();
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

		unsigned long long curtime = Time::NanoSeconds(&ts);

		std::multimap<unsigned long long, ITimer *>::iterator it = m_timers.begin();
		if (it == m_timers.end())
		{
			m_mutex.Wait(); //Wait forever on empty list until told otherwise
			continue; //Restart
		}

		if (curtime < it->first)
		{
			unsigned long long diff = it->first - curtime;
			struct timespec delay;
			Time::TimeSpecFromNanoSeconds(diff, &delay);
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

