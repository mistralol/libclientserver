
#include <libclientserver.h>


ThreadPool::ThreadPool()
{
	Init(5, 64);
}

ThreadPool::ThreadPool(int nthread, size_t maxqueue)
{
	Init(nthread, maxqueue);
}

ThreadPool::~ThreadPool()
{
	Flush();
	while(m_threads.size() > 0)
	{
		ThreadPoolThread *thd = m_threads.front();
		m_threads.pop_front();
		delete thd;
	}
}

void ThreadPool::Init(int nthread, size_t maxqueue)
{
	for(int i =0;i<nthread;i++)
	{
		ThreadPoolThread *thd = new ThreadPoolThread(this);
		thd->Start();
		m_threads.push_back(thd);
	}
	m_queue.SetMaxItems(maxqueue);
}

void ThreadPool::Add( void (*fp) (void *arg), void *arg )
{
	ThreadPoolItem *tmp = new ThreadPoolItem;
	tmp->fp = fp;
	tmp->arg = arg;
	m_queue.Add(tmp);
}

void ThreadPool::Flush()
{
	m_queue.Flush();
}

ThreadPoolItem *ThreadPool::GetNext()
{
	struct timespec ts = {1, 0};
	return m_queue.GetNext(&ts);
}

