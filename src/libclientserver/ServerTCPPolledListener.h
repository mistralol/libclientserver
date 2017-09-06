
class ServerTCPPolledListener : public IPollable
{
	public:
		ServerTCPPolledListener(ServerTCPPolled *Server);
		~ServerTCPPolledListener();

		void Init(int port, const std::string &addr);

		bool CanRead(const Poller *);
		bool CanWrite(const Poller *);
		bool CanExcept(const Poller *);
		bool CanTimeout(const Poller *);

		void DoRead(Poller *sel);
		void DoWrite(Poller *);
		void DoExcept(Poller *);
		void DoTimeout(Poller *);
		void DoClose(Poller *);

		int GetFD(const Poller *);

		void GetTimeout(const Poller *, struct timespec *tv);

	private:
		ServerTCPPolled *m_Server;
		int m_fd;
		int m_port;
		std::string m_addr;
};

