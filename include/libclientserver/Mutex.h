
/**
 * @class Mutex
 * @brief Class To support Mutex and Mutex releated operations
 *
 * Implementes a Mutex. This version it really a wrapper around the pthread mutex
 * and condition operations. The wrapper exists so that profiling / benchmark and tracing
 * can be added if it is required to support debugging later.
 *
 * Currently the class will do basic checking if DEBUG is defined. eg trying to unlock a mutex that is not locked.
 */
class Mutex {
	
	public:
		Mutex();
		~Mutex();
		
		void Lock();
		int TryLock();
		int TimedLock(const struct timespec *Timeout);
		void Unlock();

		void Wait();
		int Wait(const struct timespec *Timeout);
		void WakeUp();
		void WakeUpAll();


	protected:
		pthread_mutex_t m_mutex;
		pthread_cond_t m_cond;
		bool m_locked;
};


