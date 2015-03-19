
#include <libclientserver.h>

ServerUnixSelectedConnection::ServerUnixSelectedConnection(ServerManager *Manager, IServer *Server, int fd)
{
	m_Manager = Manager;
	m_Server = Server;
	m_fd = fd;

	if (m_ReadBuffer.Init() < 0)
		abort();

	if (m_WriteBuffer.Init() < 0)
		abort();

	if (fcntl(fd, F_SETFL, O_NONBLOCK) < 0)
		abort();

	Server->ConnectionAdd(this);
	Manager->RaisePostNewConnection(this);
}

ServerUnixSelectedConnection::~ServerUnixSelectedConnection()
{
	if (close(m_fd) < 0)
		abort();
}

bool ServerUnixSelectedConnection::CanRead(const Selector *)
{
	return true;
}

bool ServerUnixSelectedConnection::CanWrite(const Selector *)
{
	ScopedLock lock = ScopedLock(&m_WriteMutex);
	if (m_WriteBuffer.GetDataLength() > 0)
		return true;
	return false;
}

bool ServerUnixSelectedConnection::CanExcept(const Selector *)
{
	return false;
}

bool ServerUnixSelectedConnection::CanTimeout(const Selector *)
{
	return true;
}

void ServerUnixSelectedConnection::DoRead(Selector *sel)
{
	if (m_ReadBuffer.Read(m_fd) < 0)
	{
		Destroy(sel);
		return;
	}

	//Read Line at a time
	std::string line;
	bool HasLine = m_ReadBuffer.GetLine(&line);
	while(HasLine == true)
	{
		m_Manager->ProcessLine(this, &line);
		HasLine = m_ReadBuffer.GetLine(&line);
	}
}

void ServerUnixSelectedConnection::DoWrite(Selector *sel)
{
	ScopedLock lock = ScopedLock(&m_WriteMutex);
	if (m_WriteBuffer.Write(m_fd) < 0)
	{
		Destroy(sel);
		return;
	}
}

void ServerUnixSelectedConnection::DoExcept(Selector *)
{
	abort(); //Should never be called
}

void ServerUnixSelectedConnection::DoTimeout(Selector *sel)
{
	Destroy(sel);
}

int ServerUnixSelectedConnection::GetFD(const Selector *)
{
	return m_fd;
}

void ServerUnixSelectedConnection::GetTimeout(const Selector *, struct timespec *tv)
{
	tv->tv_sec = 300;
	tv->tv_nsec = 0;
}

void ServerUnixSelectedConnection::Destroy(Selector *sel)
{
	sel->Remove(this);
	m_Server->ConnectionRemove(this);
	m_Manager->RaiseDisconnect(this);
	delete this;
}

bool ServerUnixSelectedConnection::SendLine(const std::string *str)
{
	ScopedLock lock = ScopedLock(&m_WriteMutex);
	if (m_WriteBuffer.PushData(str->c_str(), str->size()) < 0)
		return false;
	return true;
}

