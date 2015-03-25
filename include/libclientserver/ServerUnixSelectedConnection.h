

class ServerUnixSelectedConnection : public IServerConnection, public ISelectable, Thread
{
	public:
		ServerUnixSelectedConnection(ServerManager *Manager, IServer *Server, int fd);
		virtual ~ServerUnixSelectedConnection();

		bool CanRead(const Selector *);
		bool CanWrite(const Selector *);
		bool CanExcept(const Selector *);
		bool CanTimeout(const Selector *);

		void DoRead(Selector *);
		void DoWrite(Selector *);
		void DoExcept(Selector *);
		void DoTimeout(Selector *);

		int GetFD(const Selector *);
		void GetTimeout(const Selector *, struct timespec *tv);

		void Destroy(Selector *);
		bool SendLine(const std::string *str);


	private:
		ServerManager *m_Manager;
		IServer *m_Server;
		int m_fd;
		Buffer m_ReadBuffer;

		Mutex m_WriteMutex;
		Buffer m_WriteBuffer;
};

