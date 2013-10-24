

class ServerUnix : public IServer, protected Thread
{
	public:
		ServerUnix(const std::string &path);
		ServerUnix(const std::string &path, mode_t perm);
		virtual ~ServerUnix();
	
		virtual void Start(ServerManager *Manager);
		virtual void Stop();
		virtual void Run();
	
	protected:
		void Init();
	
	private:
		std::string m_path;
		mode_t m_perms;
		int m_fd;
		int m_backlog;
		bool m_quit;
		ServerManager *m_manager;
		
};		

