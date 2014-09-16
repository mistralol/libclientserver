
class TimerFunc : public ITimer
{
	public:
		TimerFunc(int seconds, void (*func) (void *), void *arg);

		time_t GetDelay();
		void TimerExpired(Timers *timers, ITimer *timer);

	private:
		int m_seconds;
		void (*m_func) (void *);
		void *m_arg;
		
};

