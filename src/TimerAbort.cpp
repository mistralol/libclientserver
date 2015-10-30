
#include <libclientserver.h>

TimerAbort::TimerAbort(int seconds)
{
	m_timeout.tv_sec = seconds;
	m_timeout.tv_nsec = 0;
}

void TimerAbort::GetDelay(struct timespec *ts)
{
	*ts = m_timeout;
}

void TimerAbort::TimerExpired(Timers *timers, ITimer *timer)
{
	abort();
}

