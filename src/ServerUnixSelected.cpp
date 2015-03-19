
#include <libclientserver.h>



ServerUnixSelected::ServerUnixSelected(const std::string &path)
{
	m_path = path;
	m_perm = S_IRUSR | S_IWUSR;
	m_Listener = NULL;
	m_Selector = NULL;
}

ServerUnixSelected::ServerUnixSelected(const std::string &path, mode_t perm)
{
	m_path = path;
	m_perm = perm;
	m_Listener = NULL;
	m_Selector = NULL;
}

ServerUnixSelected::~ServerUnixSelected()
{
	if (m_Selector != NULL)
		abort(); //forgot to call Stop
}

void ServerUnixSelected::Start(ServerManager *Manager)
{
	m_Manager = Manager;
	m_Selector = new Selector();
	m_Listener = new ServerUnixSelectedListener(this);
	m_Listener->Init(m_path, m_perm);
	m_Selector->Add(m_Listener);
}

void ServerUnixSelected::Stop()
{
	m_Selector->Remove(m_Listener);
	delete m_Listener;
	m_Listener = NULL;

	delete m_Selector;
	m_Selector = NULL;

	m_Manager = NULL;
}

void ServerUnixSelected::CreateNewConnection(int fd)
{
	m_Manager->RaisePreNewConnection();
	ServerUnixSelectedConnection *tmp = new ServerUnixSelectedConnection(m_Manager, this, fd);
	m_Selector->Add(tmp);
}

