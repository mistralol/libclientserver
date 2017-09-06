

class ServerPolledConnection : public IServerConnection, public IPollable, Thread
{
	public:
		ServerPolledConnection(ServerManager *Manager, IServer *Server, Poller *poller, int fd);
		virtual ~ServerPolledConnection();

		bool CanRead(const Poller *);
		bool CanWrite(const Poller *);
		bool CanExcept(const Poller *);
		bool CanTimeout(const Poller *);

		void DoRead(Poller *);
		void DoWrite(Poller *);
		void DoExcept(Poller *);
		void DoTimeout(Poller *);
		void DoClose(Poller *);

		int GetFD(const Poller *);
		void GetTimeout(const Poller *, struct timespec *tv);

		void Destroy(Poller *);
		bool SendLine(const std::string *str);


	private:
		ServerManager *m_Manager;
		IServer *m_Server;
		Poller *m_poller;
		int m_fd;
		Buffer m_ReadBuffer;

		Mutex m_WriteMutex;
		Buffer m_WriteBuffer;
};


