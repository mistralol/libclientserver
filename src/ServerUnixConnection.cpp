
#include <libclientserver.h>

ServerUnixConnection::ServerUnixConnection(ServerManager *Manager, IServer *Server, int fd)
{
	m_manager = Manager;
	m_server = Server;
	m_fd = fd;
	m_quit = true;
}

ServerUnixConnection::~ServerUnixConnection()
{
	if (IsRunning())
		abort();
}

void ServerUnixConnection::Start()
{
	m_quit = false;
	m_manager->ConnectionAdd(this);

	Thread::Start();
	Thread::Detach();
}

void ServerUnixConnection::Stop()
{
	m_quit = true;
	m_manager->ConnectionRemove(this);
	if (close(m_fd) < 0)
		abort();	//Possible Race Here?
}

void ServerUnixConnection::Run()
{
	while(m_quit == false)
	{
		abort(); //Work Required
	}
}


