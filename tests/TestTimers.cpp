
#include <libclientserver.h>

volatile bool deleted = false;
volatile bool fired = false;

class MyFireTimer : public ITimer
{
	public:
		int m_id;
		MyFireTimer(int id)
		{
			m_id = id;
		}

		time_t GetDelay()
		{
			return m_id;
		}

		void TimerExpired(Timers *timers, ITimer *timer)
		{
			printf("Timer Deleted: %d\n", m_id);
			fired = true;
		}
};

class MyDelTimer : public ITimer
{
	public:
		int m_id;
		MyDelTimer(int id)
		{
			m_id = id;
		}

		time_t GetDelay()
		{
			return m_id;
		}

		void TimerExpired(Timers *timers, ITimer *timer)
		{
			printf("Timer Deleted: %d\n", m_id);
			deleted = true;
			delete this;
		}
};

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
	MyTimer x(1);
	
	Tmr.Start();
	
	if (alarm(30) < 0)
		abort();

	MyFireTimer *f = new MyFireTimer(1);
	Tmr.Add(f);
	while(fired == false) { }
	delete f;
	printf("Passed Fire\n");

	Tmr.Add(new MyDelTimer(1));
	while(deleted == false) { }
	printf("Passed Deleted\n");

	//Search for some races
	for(int i =0;i<50;i++)
	{
		Tmr.Add(&x);
		Tmr.Remove(&x);
	}

	sleep(1);

	Tmr.Stop();

	printf("Passed Races\n");

	printf("Cleanup\n");
	return 0;	
}
