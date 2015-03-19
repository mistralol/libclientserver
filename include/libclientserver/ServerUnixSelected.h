
class ServerUnixSelectedListener;

class ServerUnixSelected : public IServer
{
	public:
		ServerUnixSelected(const std::string &path);
		ServerUnixSelected(const std::string &path, mode_t perm);
		~ServerUnixSelected();

		void Start(ServerManager *Manager);
		void Stop();

		void CreateNewConnection(int m_fd);

	private:
		ServerManager *m_Manager;
		ServerUnixSelectedListener *m_Listener;
		Selector *m_Selector;
		std::string m_path;
		mode_t m_perm;

};

