
class ServerTCPSelectedListener;

class ServerTCPSelected : public IServer
{
	public:
		ServerTCPSelected(int port, const std::string &address = "127.0.0.1");
		~ServerTCPSelected();

		void Start(ServerManager *Manager);
		void Stop();

		void CreateNewConnection(int m_fd);

	private:
		ServerManager *m_Manager;
		ServerTCPSelectedListener *m_Listener;
		Selector *m_Selector;
		int m_port;
		std::string m_addr;
};

