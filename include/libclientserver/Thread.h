

class Thread
{
	public:
		Thread();
		virtual ~Thread();
		
		void Start();
		void Stop();
		
		bool IsRunning();
	
	protected:
		virtual void Run();
		static void RunInternal(Thread *self);
	
	private:
		bool m_IsRunning;
		boost::thread *m_thread;

};

