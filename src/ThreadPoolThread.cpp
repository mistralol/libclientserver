
#include <libclientserver.h>

ThreadPoolThread::ThreadPoolThread(ThreadPool *pool)
{
	m_exit = false;
	m_Pool = pool;
}

ThreadPoolThread::~ThreadPoolThread()
{
	Stop();
}

void ThreadPoolThread::Stop()
{
	Thread::Stop();
}

void ThreadPoolThread::SetExit() {
	m_exit = true;
}

void ThreadPoolThread::Run()
{
	while(m_exit == false)
	{
		m_Pool->Execute();
	}
}

