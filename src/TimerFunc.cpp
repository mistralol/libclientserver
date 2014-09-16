
#include <libclientserver.h>

TimerFunc::TimerFunc(int seconds, void (*func) (void *), void *arg)
{
	m_seconds = seconds;
	m_func = func;
	m_arg = arg;
}

time_t TimerFunc::GetDelay()
{
	return m_seconds;
}

void TimerFunc::TimerExpired(Timers *timers, ITimer *timer)
{
	m_func(m_arg);
}

