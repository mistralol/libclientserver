
#include <libclientserver.h>

RWLock::RWLock()
{
	if (pthread_rwlock_init(&m_lock, NULL) != 0)
		abort();
}

RWLock::~RWLock()
{
	if (pthread_rwlock_destroy(&m_lock) != 0)
		abort();
}

void RWLock::ReadLock()
{
	if (pthread_rwlock_rdlock(&m_lock) != 0)
		abort();
}

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

void RWLock::WriteLock()
{
	if (pthread_rwlock_wrlock(&m_lock) != 0)
		abort();
}

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

void RWLock::Unlock()
{
	if (pthread_rwlock_unlock(&m_lock) != 0)
		abort();
}


