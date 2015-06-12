
/** 
 * @class RWLock
 * @brief A Lock providing multiple readers and a single exclusive writer
 * A Lock providing multiple readers and a single exclusive writer
 */

class RWLock
{
	public:
		RWLock();
		~RWLock();

		void ReadLock();
		bool TryReadLock();
		bool TimeReadLock(const struct timespec *timeout);

		void WriteLock();
		bool TryWriteLock();
		bool TimeWriteLock(const struct timespec *timeout);

		void Unlock();

	private:
		pthread_rwlock_t m_lock;
};

