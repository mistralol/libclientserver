
class TimerAbort : public ITimer
{
	public:
		TimerAbort(int seconds);

		time_t GetDelay();
		void TimerExpired(Timers *timers, ITimer *timer);

	private:
		int m_seconds;
		
};


