
class ThreadPoolThread;

class ThreadPool {
	public:
		ThreadPool();
		ThreadPool(int nthread);
		ThreadPool(int nthread, size_t maxqueue);
		~ThreadPool();

		void Add(std::function<void()> func);
		void Flush();

		void Execute();
		
		size_t GetCount();
		size_t GetHWCount();

	protected:
		void Init(int nthread, size_t maxqueue);

	private:
		Mutex m_mutex;
		std::atomic<uint64_t> m_totalqueued;
		std::atomic<uint64_t> m_totalexecuted;
		Queue<std::function<void()> > m_queue;
		std::list<ThreadPoolThread *> m_threads;
};

