
struct ThreadPoolItem {
	void (*fp) (void *arg);
	void *arg;
};

class ThreadPoolThread;

class ThreadPool {
	public:
		ThreadPool();
		ThreadPool(int nthread, size_t maxqueue);
		~ThreadPool();

		void Add( void (*fp) (void *arg), void *arg );
		void Flush();

		ThreadPoolItem *GetNext();
	protected:
		void Init(int nthread, size_t maxqueue);

	private:
		Mutex m_mutex;
		Queue<ThreadPoolItem *> m_queue;
		std::list<ThreadPoolThread *> m_threads;
};


