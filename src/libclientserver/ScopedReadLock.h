
/**
 * @class ScopedReadLock
 * @brief Proved a class which gives read-only access to a RWLock
 *
 * Used to shorten Lock / Unlock code and make it nearly impossible
 * to leave a mutex locked. eg when an exception occurs.
 *
 * This class also support the ability to Unlock early if it is required.
 */

class ScopedReadLock
{
	public:
		ScopedReadLock(RWLock *lock);
		~ScopedReadLock();

		void Unlock();

	private:
		RWLock *m_lock;
		bool m_locked;
};

