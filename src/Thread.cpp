

#include <libclientserver.h>


Thread::Thread()
{
	m_IsRunning = false;
	m_IsDetached = false;
}

Thread::~Thread()
{
	if (IsRunning())
		Stop();

}

void Thread::Start()
{
	m_IsRunning = true;
	if (pthread_create(&m_thread, NULL, RunInternal, this) != 0)
		abort();
}

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
}

void Thread::Detach()
{
	if (pthread_detach(m_thread) != 0)
		abort();
	m_IsDetached = true;
}

bool Thread::IsRunning()
{
	return m_IsRunning;
}

bool Thread::IsDetached()
{
	return m_IsDetached;
}

void Thread::Run()
{
	abort();	//You should override this function
}

void *Thread::RunInternal(void *ptr)
{
	class Thread *self = (Thread *) ptr;
	self->Run();
	return NULL;
}

