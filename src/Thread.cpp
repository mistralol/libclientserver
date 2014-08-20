

#include <libclientserver.h>


Thread::Thread()
{
	m_IsRunning = false;
	m_IsDetached = false;
}

Thread::~Thread()
{
	if (IsRunning() && (IsDetached() == false))
		Stop();
}

/**
 * Start
 *
 * Start the thread running
 */
void Thread::Start()
{
	m_IsRunning = true;
	if (pthread_create(&m_thread, NULL, RunInternal, this) != 0)
		abort();
}

/**
 * Stop
 *
 * This will cause the thread to stop.
 * However please be aware you should signal to your thread first that it should stop executing.
 * As this is really just a cleanup function that calls join. It will not interrupt a currently executing thread.
 * It also will not return until the thread exits. If this happens it is a bug elsewhere
 */
void Thread::Stop()
{
#ifdef DEBUG
	if (IsDetached() == true)
		abort();

	if (m_IsRunning == false)
	{
		abort();	//Attempted to stop an already stopped thread
	}
#endif
	if (pthread_join(m_thread, NULL) != 0)
		abort();
	m_IsRunning = false;
}

/**
 * Detach
 *
 * This will detatch the thread.
 * If this function is called it will also be safe to destroy an instance of the thread object.
 * You may then NEVER call stop on a detatched thread as it will NEVER return
 * 
 */
void Thread::Detach()
{
	if (pthread_detach(m_thread) != 0)
		abort();
	m_IsDetached = true;
}

/**
 * IsRunning
 *
 * Reports if the thread is actually running
 */
bool Thread::IsRunning()
{
	return m_IsRunning;
}

/**
 * IsDetached
 *
 * Reports if the thread has been Detatched
 */
bool Thread::IsDetached()
{
	return m_IsDetached;
}

/**
 * IsSameThread
 *
 * Returns true if the thread is running as the current thread
 */
bool Thread::IsSameThread()
{
	if (pthread_equal(pthread_self(), m_thread) == 0)
		return false;
	return true;
}

/**
 * Signal
 *
 * Sends a specific posix signal to the specific thread
 */
void Thread::Signal(int signum)
{
	if (pthread_kill(m_thread, signum) != 0)
	{
		abort(); //This points to programing error in caller
	}
}

/**
 * Run
 *
 * This is up to the derived class to implement
 */
void Thread::Run()
{
	abort();	//You should override this function
}

/**
 * RunInternal
 *
 * Simple "bounce" function to call the Run function after starting the thread
 */
void *Thread::RunInternal(void *ptr)
{
	class Thread *self = (Thread *) ptr;
	self->Run();
	return NULL;
}

