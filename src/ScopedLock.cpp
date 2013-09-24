
#include <libclientserver.h>

ScopedLock::ScopedLock(Mutex *m) {
	m_mutex = m;
	m_mutex->Lock();
}


ScopedLock::~ScopedLock() {
	m_mutex->Unlock();
}


