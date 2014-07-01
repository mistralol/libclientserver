
class Barrier
{
	public:
		Barrier();
		~Barrier();

		void Wait();
		bool Wait(const struct timespec *Timeout);

		void WakeUp();

	private:
		bool m_fired;
		Mutex m_mutex;

};

