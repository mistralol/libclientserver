
#include <libclientserver.h>

/**
 * RWLock
 */
RWLock::RWLock()
{
	if (pthread_rwlock_init(&m_lock, NULL) != 0)
		abort();
}

/**
 * ~RWLock
 */
RWLock::~RWLock()
{
	if (pthread_rwlock_destroy(&m_lock) != 0)
		abort();
}

/**
 * ReadLock
 *
 * This will aquire a read lock which can be shared with other concurrent readers.
 * The lock is not recursive so a single thread should only aquire the lock once.
 *
 **/
void RWLock::ReadLock()
{
	if (pthread_rwlock_rdlock(&m_lock) != 0)
		abort();
}

/**
 * TryReadLock
 *
 * This will attempt to aquire the read lock. If it is not able to do so false will be returned.
 * If the lock is aquired true will be returned.
 *
 * If the lock is not aquired you should not make a call to Unlock as the lock is not owned.
 **/
bool RWLock::TryReadLock()
{
	int ret = pthread_rwlock_rdlock(&m_lock);
	if (ret == 0)
		return true;

	switch(ret)
	{
		case EBUSY:
			return false;
		default:
			abort();
	}
	return false;
}


/**
 * TimeReadLock
 * @params[in] timeout
 * 
 * This function will attempt to aquire a read lock. It will attempt to try for the timeout specific.
 * If it does aquire the lock true will be return. If the lock is not aquired false will be returned.
 *
 */
bool RWLock::TimeReadLock(const struct timespec *timeout)
{
	int ret = pthread_rwlock_timedrdlock(&m_lock, timeout);
	if (ret == 0)
		return true;

	switch(ret)
	{
		case ETIMEDOUT:
			return false;
		default:
			abort();
	}
	return false;
}

/**
 * WriteLock
 *
 * This function will take a WriteLock it will block until this condition is aquired.
 * You should take care that the current thread does not hold the read lock and this will cause a deadlock
 */
void RWLock::WriteLock()
{
	if (pthread_rwlock_wrlock(&m_lock) != 0)
		abort();
}

/**
 * TryWriteLock
 *
 * Try to aquire a write lock. This will return true if it is successfull. Otherwise false.
 * It is likly to return false is any other thread holds either the read lock or the write lock already
 *
 */
bool RWLock::TryWriteLock()
{
	int ret = pthread_rwlock_wrlock(&m_lock);
	if (ret == 0)
		return true;

	switch(ret)
	{
		case EBUSY:
			return false;
		default:
			abort();
	}
	return false;
}

/**
 * TimeWriteLock
 * @params[in] timeout
 * 
 * This function will attempt to aquire a write lock. It will attempt to try for the timeout specific.
 * If it does aquire the lock true will be return. If the lock is not aquired false will be returned.
 *
 */
bool RWLock::TimeWriteLock(const struct timespec *timeout)
{
	int ret = pthread_rwlock_timedwrlock(&m_lock, timeout);
	if (ret == 0)
		return true;

	switch(ret)
	{
		case ETIMEDOUT:
			return false;
		default:
			abort();
	}
	return false;
}

/**
 * Unlock
 *
 * The function will release any lock previous aquired by either any of the ReadLock or WriteLock functions
 * Care should be taken not to call this function unless the current thread actually currently owns the lock.
 */
void RWLock::Unlock()
{
	if (pthread_rwlock_unlock(&m_lock) != 0)
		abort();
}


