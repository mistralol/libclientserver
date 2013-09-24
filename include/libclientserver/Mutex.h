

class Mutex {
	
	public:
		Mutex();
		~Mutex();
		
		void Lock();
		int TryLock();
		int TimedLock(const struct timespec *Timeout);
		void Unlock();

		void Wait();
		int Wait(const struct timespec *abstime);
		void WakeUp();
		void WakeUpAll();


	protected:
		pthread_mutex_t m_mutex;
		pthread_cond_t m_cond;
		bool m_locked;
};


