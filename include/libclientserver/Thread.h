
/** 
 * @class Thread
 * @brief Inherit this class for adding a thread function
 * An easy to use class for making a thread on an object to background longer operations.
 */

class Thread
{
	public:
		Thread();
		virtual ~Thread();
		
		void Start();
		void Stop();

		void Detach();
		
		bool IsRunning();
		bool IsDetached();	

	protected:
		virtual void Run();
		static void RunInternal(Thread *self);
	
	private:
		bool m_IsRunning;
		bool m_IsDetached;
		boost::thread *m_thread;

};

