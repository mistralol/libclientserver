
class ServerTCPPolledListener;

class ServerTCPPolled : public IServer
{
	public:
		ServerTCPPolled(const int port, const std::string &addr = "127.0.0.1");
		~ServerTCPPolled();

		void Start(ServerManager *Manager);
		void Stop();

		void CreateNewConnection(int m_fd);

	private:
		ServerManager *m_Manager;
		ServerTCPPolledListener *m_Listener;
		Poller *m_Poller;
		int m_port;
		std::string m_addr;

};

