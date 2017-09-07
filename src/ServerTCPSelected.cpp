
#include <libclientserver.h>

ServerTCPSelected::ServerTCPSelected(int port, const std::string &address, const std::string &address6) :
	m_port(port),
	m_addr(address),
	m_addr6(address)
{
	m_Listener = NULL;
	m_Listener6 = NULL;
	m_Selector = NULL;
}

ServerTCPSelected::~ServerTCPSelected()
{
	if (m_Selector != NULL)
		abort(); //forgot to call Stop
}

void ServerTCPSelected::Start(ServerManager *Manager)
{
	m_Manager = Manager;
	m_Selector = new Selector();

	if (m_addr != "") {
		m_Listener = new ServerTCPSelectedListener(this);
		m_Listener->Init(m_port, m_addr);
		m_Selector->Add(m_Listener);
	}

	if (m_addr6 != "") {
		m_Listener6 = new ServerTCP6SelectedListener(this);
		m_Listener6->Init(m_port, m_addr6);
		m_Selector->Add(m_Listener6);
	}
}

void ServerTCPSelected::Stop()
{
	if (m_Listener) {
		m_Selector->Remove(m_Listener);
		delete m_Listener;
		m_Listener = NULL;
	}

	if (m_Listener6) {
		m_Selector->Remove(m_Listener6);
		delete m_Listener6;
		m_Listener6 = NULL;
	}

	delete m_Selector;
	m_Selector = NULL;

	m_Manager = NULL;
}

void ServerTCPSelected::CreateNewConnection(int fd)
{
	m_Manager->RaisePreNewConnection();
	ServerSelectedConnection *tmp = new ServerSelectedConnection(m_Manager, this, m_Selector, fd);
	m_Selector->Add(tmp);
}

