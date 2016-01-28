
class ServerUnixPolledListener;

class ServerUnixPolled : public IServer
{
	public:
		ServerUnixPolled(const std::string &path);
		ServerUnixPolled(const std::string &path, mode_t perm);
		~ServerUnixPolled();

		void Start(ServerManager *Manager);
		void Stop();

		void CreateNewConnection(int m_fd);

	private:
		ServerManager *m_Manager;
		ServerUnixPolledListener *m_Listener;
		Poller *m_Poller;
		std::string m_path;
		mode_t m_perm;

};

