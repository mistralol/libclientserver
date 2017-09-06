
#include <libclientserver.h>

ServerTCPPolled::ServerTCPPolled(const int port, const std::string &addr) :
	m_port(port),
	m_addr(addr)
{
	m_Listener = NULL;
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
	m_Listener = new ServerTCPPolledListener(this);
	m_Listener->Init(m_port, m_addr);
	m_Poller->Add(m_Listener);
}

void ServerTCPPolled::Stop()
{
	m_Poller->Remove(m_Listener);
	delete m_Listener;
	m_Listener = NULL;

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
