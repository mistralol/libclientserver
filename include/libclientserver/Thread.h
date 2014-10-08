
/** 
 * @class Thread
 * @brief Inherit this class for adding a thread function
 *
 * An easy to use class for making a thread on an object to background longer operations.
 *
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

		bool IsSameThread();
		void Signal(int signum);

		size_t GetStackSize();
		void SetStackSize(size_t size);

		size_t GetGuardSize();
		void SetGuardSize(size_t size);

	protected:
		virtual void Run();
		static void *RunInternal(void *);
	
	private:
		bool m_IsRunning;
		bool m_IsDetached;
		pthread_t m_thread;
		pthread_attr_t m_attr;

};

