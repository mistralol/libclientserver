
#include <libclientserver.h>

ServerUnixPolled::ServerUnixPolled(const std::string &path)
{
	m_path = path;
	m_perm = S_IRUSR | S_IWUSR;
	m_Listener = NULL;
	m_Poller = NULL;
}

ServerUnixPolled::ServerUnixPolled(const std::string &path, mode_t perm)
{
	m_path = path;
	m_perm = perm;
	m_Listener = NULL;
	m_Poller = NULL;
}

ServerUnixPolled::~ServerUnixPolled()
{
	if (m_Poller != NULL)
		abort(); //forgot to call Stop
}

void ServerUnixPolled::Start(ServerManager *Manager)
{
	m_Manager = Manager;
	m_Poller = new Poller();
	m_Listener = new ServerUnixPolledListener(this);
	m_Listener->Init(m_path, m_perm);
	m_Poller->Add(m_Listener);
}

void ServerUnixPolled::Stop()
{
	m_Poller->Remove(m_Listener);
	delete m_Listener;
	m_Listener = NULL;

	delete m_Poller;
	m_Poller = NULL;

	m_Manager = NULL;
}

void ServerUnixPolled::CreateNewConnection(int fd)
{
	m_Manager->RaisePreNewConnection();
	ServerUnixPolledConnection *tmp = new ServerUnixPolledConnection(m_Manager, this, m_Poller, fd);
	m_Poller->Add(tmp);
}

