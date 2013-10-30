
#include <libclientserver.h>

/**
 * Mutex
 *
 * Constructor for Mutex. If it fails it will call abort since these functions should *NEVER* fail.
 */
Mutex::Mutex() {
	if (pthread_mutex_init(&m_mutex, NULL) != 0)
		abort();

	if (pthread_cond_init(&m_cond, NULL) != 0)
		abort();

	m_locked = false;
}

/**
 * ~Mutex
 *
 * Destructor for Mutex. This should never fail. If it does it will call abort.
 *
 * If DEBUG is defined it will check if the Mutex is still locked.
 * If it is it will call abort as it indicates a software bug.
 */
Mutex::~Mutex() {
#ifdef DEBUG
	if (m_locked)
		abort(); //Attempts to free mutex that is locked
#endif

	if (pthread_cond_destroy(&m_cond) != 0)
		abort();

	if (pthread_mutex_destroy(&m_mutex) != 0)
		abort();
}

/**
 * Lock
 *
 * Lock the Mutex. This function should never fail. Though it may hang forever
 */
void Mutex::Lock() {
	if (pthread_mutex_lock(&m_mutex) < 0)
		abort(); //Could not lock mutex
	m_locked = true;
}

/**
 * TryLock
 * @return Will return 0 if the lock is successful. Otherwise -errno
 *
 * Will attempt to take a lock of the mutex.
 * If the mutex is already locked by another thread then the function will fail and return -errno
 * The error return should be -EBUSY for this condition. Anything else should be treated as a bug.
 */
int Mutex::TryLock() {
	int ret = pthread_mutex_lock(&m_mutex);
	if (ret < 0)
		return -errno;
	m_locked = true;
	return 0;
}

/**
 * TimedLock
 * @param[in] Timeout The length of time to wait for a timeout
 * @return Will return 0 if the lock is successful. Otherwise -errno
 *
 * Will attempt to take a lock of the mutex.
 * If the mutex is already locked by another thread then the function will sleep for the Timeout period.
 * The error returned should be -ETIMEOUT if the Timeout expires before the lock is aquired.
 * Any other error should be treated as asoftware bug
 */
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

/**
 * Unlock
 *
 * Unlocks a locked mutex
 * This function should never fail. If it does abort will be called.
 * Abort could be called if the caller attempt to unlock a mutex that is not locked.
 * Which is a software bug on the caller side.
 *
 */
void Mutex::Unlock() {
	if (pthread_mutex_unlock(&m_mutex) < 0)
		abort(); //Could not unlock mutex
	m_locked = false;
}

/**
 * Wait
 *
 * Wait until another thread calls WakeUp or WakeUpAll
 *
 * The thread must already hold the lock before this function is called.
 * If DEBUG is defined at compile time this will cause abort to be called if the Mutex is not locked.
 */
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

/**
 * Wait
 * @param[in] Timeout The length of time to wait until we return
 * @return Return 0 on success. Or -errno
 *
 * Wait until another thread calls WakeUp or WakeUpAll. Or the Timeout expires. If the Timeout is reached -ETIMEOUT will be returned.
 * 
 * The thread must already hold the lock before this function is called.
 * If DEBUG is defined at compile time this will cause abort to be called if the Mutex is not locked.
 *
 * Note: It is possbile for this function to return without WakeUp or WakeUpAll being returned.
 *
 */
int Mutex::Wait(const struct timespec *Timeout) {
#ifdef DEBUG
	if (m_locked == false)
		abort();
#endif
	struct timeval now;
	struct timespec ts1, ts2;

	if (gettimeofday(&now, NULL) < 0)
		abort();

	Time::TimeValtoTimeSpec(&now, &ts1);
	Time::Add(Timeout, &ts1, &ts2);

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
	return ret; /* Unreachable! */
}

/**
 * WakeUp
 * 
 * WakeUp a single thread that is sleeping in WakeUp
 *
 * It is not required to have the lock when this function is called.
 */
void Mutex::WakeUp() {
	int ret = pthread_cond_signal(&m_cond);
	if (ret < 0)
		abort(); //pthread_cond_signal failed
}

/**
 * WakeUp
 * 
 * WakeUp a all threads that is sleeping in WakeUp
 *
 * It is not required to have the lock when this function is called.
 */
void Mutex::WakeUpAll() {
	int ret = pthread_cond_broadcast(&m_cond);
	if (ret < 0)
		abort(); //pthread_cond_signal failed
}

