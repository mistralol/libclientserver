
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
		ReadBuffer Buffer(1024 * 2048);

		int ret = Buffer.Read(m_fd);
		if (ret <= 0)
		{
			m_manager->ConnectionRemove(this);
			m_quit = true;
			if (close(m_fd) < 0)
				abort();
			return;
		}

		//Read Line at a time
		std::string line;
		bool HasLine = Buffer.GetLine(&line);
		while(HasLine == true)
		{
			m_manager->ProcessLine(this, &line);
			HasLine = Buffer.GetLine(&line);
		}
	}
}


