
/**
 * @class ScopedWriteLock
 * @brief Proved a class which gives read / write access to a RWLock
 *
 * Used to shorten Lock / Unlock code and make it nearly impossible
 * to leave a RWLock locked. eg when an exception occurs.
 *
 * This class also support the ability to Unlock early if it is required.
 */

class ScopedWriteLock
{
	public:
		ScopedWriteLock(RWLock *lock);
		~ScopedWriteLock();

		void Unlock();

	private:
		RWLock *m_lock;
		bool m_locked;
};

