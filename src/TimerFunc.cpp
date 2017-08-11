
#include <libclientserver.h>

TimerFunc::TimerFunc(int seconds, std::function<void()> func)
{
	m_timeout.tv_sec = seconds;
	m_timeout.tv_nsec = 0;
	m_func = func;
}

void TimerFunc::GetDelay(struct timespec *ts)
{
	*ts = m_timeout;
}

void TimerFunc::TimerExpired(Timers *timers, ITimer *timer)
{
	m_func();
}

