
class ThreadPoolThread : public Thread {
	public:
		ThreadPoolThread(ThreadPool *);
		~ThreadPoolThread();

		void Stop();
		void Run();

	private:
		ThreadPool *m_Pool;
		bool m_exit;
};

