
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
	ScopedWriteLock wlock(&m_WriterLock);	//We need to protect m_fd from the writer side while we close it
	if (close(m_fd) < 0)
		abort();
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
			ScopedWriteLock wlock(&m_WriterLock);	//We need to protect m_fd from the writer side while we close it
			if (close(m_fd) < 0)
				abort();
			m_fd = -1;
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

bool ServerUnixConnection::SendLine(const std::string *str)
{
	ScopedReadLock rlock(&m_WriterLock);
	int err = write(m_fd, str->c_str(), str->size());
	//FIXME: Deal with partial write
	if (err == (int) str->length())
		return true;

	return false;
}



