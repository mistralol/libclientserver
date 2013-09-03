

class ServerUnix : public IServer, protected Thread
{
	public:
		ServerUnix(const std::string &path);
		virtual ~ServerUnix();
	
		virtual void Start(ServerManager *Manager);
		virtual void Stop();
		virtual void Run();
	
	private:
		std::string m_path;
		int m_fd;
		int m_backlog;
		bool m_quit;
		ServerManager *m_manager;
		
};		

