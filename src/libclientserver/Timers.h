

class Timers : protected Thread
{
	public:
		void Start();
		void Stop();
		
		void Add(ITimer *timer);
		void Remove(ITimer *timer);

	private:

		void Run();

	private:
		std::multimap<unsigned long long, ITimer *> m_timers;
		bool m_run;
		Mutex m_mutex;
};


