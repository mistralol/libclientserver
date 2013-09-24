
#include <libclientserver.h>

Mutex::Mutex() {
	m_mutex = PTHREAD_MUTEX_INITIALIZER;
	m_locked = false;
	
}

Mutex::~Mutex() {
	if (m_locked)
		abort(); //Attempts to free mutex that is locked
}
		
void Mutex::Lock() {
	if (pthread_mutex_lock(&m_mutex) < 0)
		abort(); //Could not lock mutex
	m_locked = true;
}

int Mutex::TryLock() {
	int ret = pthread_mutex_lock(&m_mutex);
	if (ret < 0)
		return -errno;
	m_locked = true;
	return 0;
}

int Mutex::TimedLock(const struct timespec *Timeout)
{
	int ret = pthread_mutex_timedlock(&m_mutex, Timeout);
	if (ret == 0)
	{
		m_locked = true;
		return 0;
	}
	return -errno;
}

void Mutex::Unlock() {
	if (pthread_mutex_unlock(&m_mutex) < 0)
		abort(); //Could not unlock mutex
	m_locked = false;
}

void Mutex::Wait() {
	m_locked = false;
	int ret = pthread_cond_wait(&m_cond, &m_mutex);
	if (ret < 0)
		abort(); //pthread_cond_wait failed
	m_locked = true;
}

int Mutex::Wait(const struct timespec *abstime) {
	m_locked = false;
	int ret = pthread_cond_timedwait(&m_cond, &m_mutex, abstime);
	if (ret < 0) {
		switch(errno) {
			case ETIMEDOUT:
				ret = -errno;
				break;
			default:
				abort(); //pthread_cond_timedwait failed
				break;
		}
	}
	m_locked = true;
	return ret;
}

void Mutex::WakeUp() {
	int ret = pthread_cond_signal(&m_cond);
	if (ret < 0)
		abort(); //pthread_cond_signal failed
}

void Mutex::WakeUpAll() {
	int ret = pthread_cond_broadcast(&m_cond);
	if (ret < 0)
		abort(); //pthread_cond_signal failed
}

