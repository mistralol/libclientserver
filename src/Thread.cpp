

#include <libclientserver.h>


Thread::Thread()
{
	m_IsRunning = false;
}

Thread::~Thread()
{
	if (IsRunning())
		Stop();
}

void Thread::Start()
{
	m_IsRunning = true;
	m_thread = new boost::thread( boost::bind (RunInternal, this) );
}

void Thread::Stop()
{
#ifdef DEBUG
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

bool Thread::IsRunning()
{
	return m_IsRunning;
}

void Thread::Run()
{
	abort();	//You should override this function
}

void Thread::RunInternal(Thread *self)
{
	self->Run();
}

