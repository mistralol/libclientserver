
#include <libclientserver.h>

Timers::Timers()
{
	m_run = true;
	Thread::Start();
}

Timers::~Timers()
{
	if (m_timers.size() != 0)
		abort(); //We still have active timers
	m_run = false;
	m_mutex.Lock();
	m_mutex.WakeUp(); //Tell thread to wakeup and exit
	m_mutex.Unlock();
	Thread::Stop();
}

void Timers::Add(ITimer *timer)
{
	struct timespec ts;
	Time::MonoTonic(&ts);

	struct timespec timeout;
	timer->GetDelay(&timeout);

	struct timespec expire;
	Time::Add(&ts, &timeout, &expire);

	uint64_t curtime = Time::NanoSeconds(&expire);
	ScopedLock lock(&m_mutex);
	m_timers.insert( std::pair<uint64_t, ITimer *>(curtime, timer) );
	m_mutex.WakeUp();
}

void Timers::Remove(ITimer *timer)
{
	ScopedLock lock(&m_mutex);
	std::multimap<uint64_t, ITimer *>::iterator it = m_timers.begin();
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
		Time::MonoTonic(&ts);

		uint64_t curtime = Time::NanoSeconds(&ts);

		std::multimap<uint64_t, ITimer *>::iterator it = m_timers.begin();
		if (it == m_timers.end())
		{
			m_mutex.Wait(); //Wait forever on empty list until told otherwise
			continue; //Restart
		}

		if (curtime < it->first)
		{
			uint64_t diff = it->first - curtime;
			struct timespec delay;
			Time::TimeSpecFromNanoSeconds(diff, &delay);
			m_mutex.Wait(&delay);
		}

		//We were asleep an unlocked. So thing things may have changed.
		it = m_timers.begin();
		if (it == m_timers.end())
			continue; //All timers we removed while we were sleeping

		//Get current time *again*
		Time::MonoTonic(&ts);
		curtime = Time::NanoSeconds(&ts);

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

