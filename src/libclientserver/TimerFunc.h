
class TimerFunc : public ITimer
{
	public:
		TimerFunc(int seconds, std::function<void()> func);

		void GetDelay(struct timespec *ts);
		void TimerExpired(Timers *timers, ITimer *timer);

	private:
		struct timespec m_timeout;
		std::function<void()> m_func;
};

