

#include <libclientserver.h>

ScopedWriteLock::ScopedWriteLock(RWLock *lock)
{
	lock->WriteLock();
	m_lock = lock;
	m_locked = true;
}

ScopedWriteLock::~ScopedWriteLock()
{
	if (m_locked)
		m_lock->Unlock();
}

void ScopedWriteLock::Unlock()
{
	m_lock->Unlock();
	m_locked = false;
}


