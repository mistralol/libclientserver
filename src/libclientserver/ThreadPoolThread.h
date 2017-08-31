
class ThreadPoolThread : public Thread {
	public:
		ThreadPoolThread(ThreadPool *);
		~ThreadPoolThread();

		void Stop();
		void SetExit();
		void Run();

private:
		ThreadPool *m_Pool;
		volatile bool m_exit;
};

