
#include <libclientserver.h>

TimerAbort::TimerAbort(int seconds)
{
	m_seconds = seconds;
}

time_t TimerAbort::GetDelay()
{
	return m_seconds;
}

void TimerAbort::TimerExpired(Timers *timers, ITimer *timer)
{
	abort();
}

