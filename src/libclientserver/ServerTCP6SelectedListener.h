
class ServerTCP6SelectedListener : public ISelectable
{
	public:
		ServerTCP6SelectedListener(ServerTCPSelected *Server);
		~ServerTCP6SelectedListener();

		void Init(int port, const std::string &address);

		bool CanRead(const Selector *);
		bool CanWrite(const Selector *);
		bool CanExcept(const Selector *);
		bool CanTimeout(const Selector *);

		void DoRead(Selector *sel);
		void DoWrite(Selector *);
		void DoExcept(Selector *);
		void DoTimeout(Selector *);
		void DoClose(Selector *);

		int GetFD(const Selector *);

		void GetTimeout(const Selector *, struct timespec *tv);

	private:
		ServerTCPSelected *m_Server;
		int m_port;
		std::string m_addr;
		int m_fd;
};

