

class Timers;

class ITimer
{
	public:
		virtual ~ITimer() { }

		virtual time_t GetDelay() = 0;
		virtual void TimerExpired(Timers *timers, ITimer *timer) = 0;
};


