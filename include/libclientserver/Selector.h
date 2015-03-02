

/** 
 * @class Selector
 * @brief A wrapper around the posix syscall select
 *
 * Used to drive multiple objects from file descriptors.
 *
 * Some care must be taken to use this class correctly. Some hints are below.
 *
 * When a class is added it must return a valid file descriptor. If it does not the program will abort.
 *
 * After a class is added to the select engine its file descriptor must not change until the class is removed from the selector.
 *
 * Classes can be added / remove from within the interface calls. The permissions of what can be done are controlled by the const.
 *
 * It is safe to remove and delete and instance of the class from the DoRead, DoWrite, DoExcept, DoTimeout functions
 */

class Selector : private Thread
{
	public:
		Selector();
		~Selector();

		void Add(ISelectable *p);
		void Update(ISelectable *p);
		void Remove(ISelectable *p);
	
	protected:
		void FindHighestFD();
		void WakeUp();
		void UpdateMap(int fd);
		void ReadControl();
		void CalcTimeout(struct timespec *tv);
		void Run();

	private:
		Mutex m_mutex;
		bool m_loop;
		int m_controlfd;
		int m_maxfd;
		std::map<int, ISelectable *> m_map;
		std::map<int, struct timespec> m_timeout;
		fd_set m_freads;
		fd_set m_fwrites;
		fd_set m_fexcept;
		bool m_modified;

};

