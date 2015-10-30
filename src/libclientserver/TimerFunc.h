
class TimerFunc : public ITimer
{
	public:
		TimerFunc(int seconds, void (*func) (void *), void *arg);

		void GetDelay(struct timespec *ts);
		void TimerExpired(Timers *timers, ITimer *timer);

	private:
		struct timespec m_timeout;
		void (*m_func) (void *);
		void *m_arg;
		
};

