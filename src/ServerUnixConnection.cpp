
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
		Stop();
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
	ScopedWriteLock wlock(&m_WriterLock);	//We need to protect m_fd from the writer side while we close it
	if (close(m_fd) < 0)
		abort();

	Thread::Stop();
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
	delete this;
}

bool ServerUnixConnection::SendLine(const std::string *str)
{
	ScopedReadLock rlock(&m_WriterLock);

	const char *c = str->c_str();
	size_t offset = 0;
	size_t length = str->size();
	size_t ret = 0;

restart:
	ret = write(m_fd, &c[offset], length);
	if (ret < 0)
	{
		switch(errno)
		{
			case EINTR:
				goto restart;
				break;
			default:
				return false;
		}
	}

	if (ret < length - offset)
	{
		length -= ret;
		offset += ret;
		goto restart;
	}

	//Success!
	return false;
}



