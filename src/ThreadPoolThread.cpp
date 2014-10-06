
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
	m_exit = true;
	Thread::Stop();
}

void ThreadPoolThread::Run()
{
	while(m_exit == false)
	{
		ThreadPoolItem *item = m_Pool->GetNext();
		if (item == NULL)
			continue;
		item->fp(item->arg);
		delete item;
	}
}

