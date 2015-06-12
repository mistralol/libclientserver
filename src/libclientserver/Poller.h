

/** 
 * @class Poller
 * @brief A wrapper around the posix syscall ppoll
 *
 * Used to drive multiple objects from file descriptors.
 *
 * Some care must be taken to use this class correctly. Some hints are below.
 *
 * When a class is added it must return a valid file descriptor. If it does not the program will abort.
 *
 * After a class is added to the select engine its file descriptor must not change until the class is removed from the poller.
 * so the correct way to change a file descriptor would be to Remove(this); ChangeFD, Add(this)
 *
 * Classes can be added / remove from within the interface calls. The permissions of what can be done are controlled by the const.
 *
 * It is safe to remove and delete and instance of the class from the DoRead, DoWrite, DoExcept, DoTimeout functions
 *
 * None of the interface calls should block. If a block should occur then the select engine will fail to process data flow for other file descriptors
 *
 * Every time one of DoRead, DoWrite, DoExcept, DoTimeout functions are called. The CanRead, CanWrite, CanExcept, CanTimeout
 * functions will be checked after all of the event calsl are made.
 */

class Poller : private Thread
{
	public:
		Poller();
		~Poller();

		void Add(IPollable *p);
		void Update(IPollable *p);
		void Remove(IPollable *p);
	
	protected:
		void WakeUp();
		void UpdateMap(int fd);
		void ReadControl();
		void CalcTimeout(struct timespec *tv);
		void Run();

	private:
		Mutex m_mutex;
		bool m_loop;
		int m_controlfd;
		std::map<int, IPollable *> m_map;
		std::map<int, struct timespec> m_timeout;
		bool m_modified;
		int m_err_ebadf;
};


