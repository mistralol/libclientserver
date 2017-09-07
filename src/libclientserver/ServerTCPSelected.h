
class ServerTCPSelectedListener;
class ServerTCP6SelectedListener;

class ServerTCPSelected : public IServer
{
	public:
		ServerTCPSelected(int port, const std::string &address = "127.0.0.1", const std::string &address6 = "::1");
		~ServerTCPSelected();

		void Start(ServerManager *Manager);
		void Stop();

		void CreateNewConnection(int m_fd);

	private:
		ServerManager *m_Manager;
		ServerTCPSelectedListener *m_Listener;
		ServerTCP6SelectedListener *m_Listener6;
		Selector *m_Selector;
		int m_port;
		std::string m_addr;
		std::string m_addr6;
};

