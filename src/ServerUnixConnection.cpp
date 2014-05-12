
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
	m_server->ConnectionAdd(this);
	m_manager->RaisePostNewConnection(this);
	ScopedWriteLock wlock(&m_WriterLock);
	Thread::Start();
	Thread::Detach();
}

void ServerUnixConnection::Stop()
{
	m_quit = true;
	ScopedWriteLock wlock(&m_WriterLock);	//We need to protect m_fd from the writer side while we close it
	if (m_fd >= 0)
		shutdown(m_fd, SHUT_WR); //This error really doesnt matter

}

void ServerUnixConnection::Run()
{
	while(m_quit == false)
	{
		ReadBuffer Buffer(1024 * 2048);

		int ret = Buffer.Read(m_fd);
		if (ret <= 0)
		{
			m_server->ConnectionRemove(this);
			m_quit = true;
			ScopedWriteLock wlock(&m_WriterLock);	//We need to protect m_fd from the writer side while we close it
			if (close(m_fd) < 0)
				abort();
			m_fd = -1;
			goto doexit;
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

doexit:
	m_server->ConnectionRemove(this);
	m_manager->RaiseDisconnect(this);

	delete this;
}

bool ServerUnixConnection::SendLine(const std::string *str)
{
	ScopedReadLock rlock(&m_WriterLock);

	const char *c = str->c_str();
	size_t offset = 0;
	size_t length = str->size();
	size_t ret = 0;

	if (m_fd < 0)
		return false;

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



