
class ServerTCPPolledListener;
class ServerTCP6PolledListener;

class ServerTCPPolled : public IServer
{
	public:
		ServerTCPPolled(const int port, const std::string &addr = "127.0.0.1", const std::string &addr6 = "::1");
		~ServerTCPPolled();

		void Start(ServerManager *Manager);
		void Stop();

		void CreateNewConnection(int m_fd);

	private:
		ServerManager *m_Manager;
		ServerTCPPolledListener *m_Listener;
		ServerTCP6PolledListener *m_Listener6;
		Poller *m_Poller;
		int m_port;
		std::string m_addr;
		std::string m_addr6;

};

