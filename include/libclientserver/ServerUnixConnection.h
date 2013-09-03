

class ServerUnixConnection : public IServerConnection, Thread
{
	public:
		ServerUnixConnection(ServerManager *Manager, IServer *Server, int fd);
		virtual ~ServerUnixConnection();

		virtual void Start();
		virtual void Stop();

	private:
		virtual void Run();

	private:
		ServerManager *m_manager;
		IServer *m_server;
		int m_fd;
		bool m_quit;

};

