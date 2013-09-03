
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

}

void ServerUnixConnection::Start()
{
	m_quit = false;
	m_manager->ConnectionAdd(this);

	Thread::Start();
}

void ServerUnixConnection::Stop()
{
	m_quit = true;
	m_manager->ConnectionRemove(this);
}

void ServerUnixConnection::Run()
{
	while(m_quit == false)
	{
		abort(); //Work Required
	}
}


