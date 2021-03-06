
/**
 * @class ServerUnix
 * @brief Implementation for server side of unix sockets
 *
 * Implements the server side of the client / server support for unix sockets.
 *
 */

class ServerUnix : public IServer, protected Thread
{
	public:
		ServerUnix(const std::string &path);
		ServerUnix(const std::string &path, mode_t perm);
		~ServerUnix();
	
		void Start(ServerManager *Manager);
		void Stop();
		void Run();
	
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

