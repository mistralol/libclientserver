

#include <libclientserver.h>

ScopedReadLock::ScopedReadLock(RWLock *lock)
{
	lock->ReadLock();
	m_lock = lock;
	m_locked = true;
}

ScopedReadLock::~ScopedReadLock()
{
	if (m_locked)
		m_lock->Unlock();
}

void ScopedReadLock::Unlock()
{
	m_lock->Unlock();
	m_locked = false;
}


