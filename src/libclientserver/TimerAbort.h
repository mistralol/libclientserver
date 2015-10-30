
class TimerAbort : public ITimer
{
	public:
		TimerAbort(int seconds);

		void GetDelay(struct timespec *ts);
		void TimerExpired(Timers *timers, ITimer *timer);

	private:
		struct timespec m_timeout;
		
};


