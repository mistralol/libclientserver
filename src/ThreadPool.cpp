
#include <libclientserver.h>


ThreadPool::ThreadPool()
{
	Init(1, 0);
}

ThreadPool::ThreadPool(int nthreads)
{
	Init(nthreads, 0);
}

ThreadPool::ThreadPool(int nthread, size_t maxqueue)
{
	Init(nthread, maxqueue);
}

ThreadPool::~ThreadPool()
{
	Flush();

	//Mark all to exit
	for(auto &it : m_threads) {
		it->SetExit();
	}

	//Make them wakeup
	for(size_t i = 0; i < m_threads.size();i++) {
		Add(std::bind([]() { }));
	}

	Flush();

	//Now reap them
	while(m_threads.size() > 0)
	{
		ThreadPoolThread *thd = m_threads.front();
		m_threads.pop_front();
		delete thd;
	}

	//The exit path is a little racy so we need to process anything else left over.
	while(m_queue.GetCount()) {
		Execute();
	}
}

void ThreadPool::Init(int nthread, size_t maxqueue)
{
	m_totalqueued = 0;
	m_totalexecuted = 0;
	for(int i =0;i<nthread;i++)
	{
		ThreadPoolThread *thd = new ThreadPoolThread(this);
		thd->Start();
		m_threads.push_back(thd);
	}
	m_queue.SetMaxItems(maxqueue);
}

void ThreadPool::Add(std::function<void()> func)
{
	m_queue.Add(func);
	m_totalqueued++;
}

void ThreadPool::Flush()
{
	uint64_t queued = m_totalqueued;
	while(queued < m_totalexecuted) {
		m_queue.Flush();
	}
}

void ThreadPool::Execute() {
	std::function<void()> func;
	m_queue.GetNext(func);
	func();
	m_totalexecuted++;
}

size_t ThreadPool::GetCount()
{
	return m_queue.GetCount();
}

size_t ThreadPool::GetHWCount()
{
	return m_queue.GetHWCount();
}


