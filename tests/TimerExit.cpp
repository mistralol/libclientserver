
#include <libclientserver.h>

class MyTimer : public ITimer
{
	public:
		int m_id;
		MyTimer(int id)
		{
			m_id = id;
		}

		void GetDelay(struct timespec *ts)
		{
			ts->tv_sec = m_id;
			ts->tv_nsec = 0;
		}

		void TimerExpired(Timers *timers, ITimer *timer)
		{
			printf("Timer Fired: %d\n", m_id);
			timers->Add(timer);
		}
};

class MyTimerExit : public ITimer
{
	public:
		void GetDelay(struct timespec *ts)
		{
			ts->tv_sec = 10;
			ts->tv_nsec = 0;
		}

		void TimerExpired(Timers *timers, ITimer *timer)
		{
			_exit(0);
		}
};

int main(int argc, char **argv)
{
	Timers Tmr;

	Tmr.Start();
	if (alarm(30) < 0)
		abort();

	Tmr.Add(new MyTimerExit());

	for(int i = 1; i <= 3 ; i++)
	{
		Tmr.Add(new MyTimer(i));
	}

	
	while(1)
	{
		sleep(1);
	}
	
	return 0;	
}
