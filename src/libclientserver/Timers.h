

class Timers : protected Thread
{
	public:
		Timers();
		~Timers();
	
		void Add(ITimer *timer);
		void Remove(ITimer *timer);

	private:

		void Run();

	private:
		std::multimap<uint64_t, ITimer *> m_timers;
		bool m_run;
		Mutex m_mutex;
};


