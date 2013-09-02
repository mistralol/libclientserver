
#include <libclientserver.h>


ServerUnix::ServerUnix(const std::string &path)
{
	m_backlog = 32;
	m_path = path;
	m_fd = -1;
	m_quit = true;
}

ServerUnix::~ServerUnix()
{
	if (IsRunning())
		abort();	//Attempted to remove an active server
}

void ServerUnix::Start(ServerManager *Manager)
{
	m_quit = false;
	Thread::Start();
}

void ServerUnix::Stop()
{
	if (close(m_fd) < 0)
		abort();
		
	Thread::Stop();
}

void ServerUnix::Run()
{
	while(m_quit == false)
	{
	
	}
}


