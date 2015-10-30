

class Timers;

class ITimer
{
	public:
		virtual ~ITimer() { }

		virtual void GetDelay(struct timespec *ts) = 0;
		virtual void TimerExpired(Timers *timers, ITimer *timer) = 0;
};


