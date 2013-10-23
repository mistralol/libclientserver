
#include <libclientserver.h>

Mutex::Mutex() {
	if (pthread_mutex_init(&m_mutex, NULL) != 0)
		abort();

	if (pthread_cond_init(&m_cond, NULL) != 0)
		abort();

	m_locked = false;
}

Mutex::~Mutex() {
	if (m_locked)
		abort(); //Attempts to free mutex that is locked

	if (pthread_cond_destroy(&m_cond) != 0)
		abort();

	if (pthread_mutex_destroy(&m_mutex) != 0)
		abort();
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
#ifdef DEBUG
	if (m_locked == false)
		abort();
#endif

	m_locked = false;
	int ret = pthread_cond_wait(&m_cond, &m_mutex);
	if (ret < 0)
		abort(); //pthread_cond_wait failed
	m_locked = true;
}

int Mutex::Wait(const struct timespec *abstime) {
#ifdef DEBUG
	if (m_locked == false)
		abort();
#endif
	struct timeval now;
	struct timespec ts1, ts2;

	if (gettimeofday(&now, NULL) < 0)
		abort();

	Time::TimeValtoTimeSpec(&now, &ts1);
	Time::Add(abstime, &ts1, &ts2);

	m_locked = false;
	int ret = pthread_cond_timedwait(&m_cond, &m_mutex, &ts2);
	m_locked = true;
	if (ret != 0) {
		switch(ret) {
			case ETIMEDOUT:
				return -ETIMEDOUT;
				break;
			case EBUSY:
			case EINTR:
				return 0;
			default:
				abort(); //pthread_cond_timedwait failed
				break;
		}
	}
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

