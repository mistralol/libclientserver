
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

};

