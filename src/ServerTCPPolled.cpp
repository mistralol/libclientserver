
#include <libclientserver.h>

ServerTCPPolled::ServerTCPPolled(const int port, const std::string &addr, const std::string &addr6) :
	m_port(port),
	m_addr(addr),
	m_addr6(addr6)
{
	m_Listener = NULL;
	m_Listener6 = NULL;
	m_Poller = NULL;
}

ServerTCPPolled::~ServerTCPPolled()
{
	if (m_Poller != NULL)
		abort(); //forgot to call Stop
}

void ServerTCPPolled::Start(ServerManager *Manager)
{
	m_Manager = Manager;
	m_Poller = new Poller();

	if (m_addr != "") {
		m_Listener = new ServerTCPPolledListener(this);
		m_Listener->Init(m_port, m_addr);
		m_Poller->Add(m_Listener);
	}

	if (m_addr6 != "") {
		m_Listener6 = new ServerTCP6PolledListener(this);
		m_Listener6->Init(m_port, m_addr6);
		m_Poller->Add(m_Listener6);
	}
}

void ServerTCPPolled::Stop()
{
	if (m_Listener) {
		m_Poller->Remove(m_Listener);
		delete m_Listener;
		m_Listener = NULL;
	}

	if (m_Listener6) {
		m_Poller->Remove(m_Listener6);
		delete m_Listener6;
		m_Listener6 = NULL;
	}

	delete m_Poller;
	m_Poller = NULL;

	m_Manager = NULL;
}

void ServerTCPPolled::CreateNewConnection(int fd)
{
	m_Manager->RaisePreNewConnection();
	ServerPolledConnection *tmp = new ServerPolledConnection(m_Manager, this, m_Poller, fd);
	m_Poller->Add(tmp);
}
