
#include <libclientserver.h>

class MyTimer : public ITimer
{
	public:
		int m_id;
		MyTimer(int id)
		{
			m_id = id;
		}

		time_t GetDelay()
		{
			return m_id;
		}

		void TimerExpired(Timers *timers, ITimer *timer)
		{
			printf("Timer Fired: %d\n", m_id);
			timers->Add(timer);
		}
};

int main(int argc, char **argv)
{
	Timers Tmr;
	MyTimer x(2);

	Tmr.Start();

	for(int i =0;i<50;i++)
	{
		Tmr.Add(&x);
		Tmr.Remove(&x);
	}

	sleep(5);

	Tmr.Stop();
	
	return 0;	
}
