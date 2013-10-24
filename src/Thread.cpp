

#include <libclientserver.h>


Thread::Thread()
{
	m_IsRunning = false;
	m_IsDetached = false;
	m_thread = NULL;
}

Thread::~Thread()
{
	if (IsRunning())
		Stop();

	delete m_thread;
}

void Thread::Start()
{
	m_IsRunning = true;
	m_thread = new boost::thread( boost::bind (RunInternal, this) );
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
	if (m_thread != NULL)
	{
		m_thread->join();
		delete m_thread;
		m_thread = NULL;
	}
}

void Thread::Detach()
{
	m_thread->detach();
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

void Thread::RunInternal(Thread *self)
{
	self->Run();
}

