
#include <libclientserver.h>

ServerPolledConnection::ServerPolledConnection(ServerManager *Manager, IServer *Server, Poller *poller, int fd)
{
	m_Manager = Manager;
	m_Server = Server;
	m_poller = poller;
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

ServerPolledConnection::~ServerPolledConnection()
{
	if (close(m_fd) < 0)
		abort();
}

bool ServerPolledConnection::CanRead(const Poller *)
{
	return true;
}

bool ServerPolledConnection::CanWrite(const Poller *)
{
	ScopedLock lock = ScopedLock(&m_WriteMutex);
	if (m_WriteBuffer.GetDataLength() > 0)
		return true;
	return false;
}

bool ServerPolledConnection::CanExcept(const Poller *)
{
	return false;
}

bool ServerPolledConnection::CanTimeout(const Poller *)
{
	return true;
}

void ServerPolledConnection::DoRead(Poller *sel)
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

void ServerPolledConnection::DoWrite(Poller *sel)
{
	ScopedLock lock = ScopedLock(&m_WriteMutex);
	if (m_WriteBuffer.Write(m_fd) < 0)
	{
		Destroy(sel);
		return;
	}
}

void ServerPolledConnection::DoExcept(Poller *)
{
	//Should never be called
}

void ServerPolledConnection::DoTimeout(Poller *sel)
{
	Destroy(sel);
}

void ServerPolledConnection::DoClose(Poller *sel)
{
	Destroy(sel);
}

int ServerPolledConnection::GetFD(const Poller *)
{
	return m_fd;
}

void ServerPolledConnection::GetTimeout(const Poller *, struct timespec *tv)
{
	tv->tv_sec = 300;
	tv->tv_nsec = 0;
}

void ServerPolledConnection::Destroy(Poller *sel)
{
	sel->Remove(this);
	m_Server->ConnectionRemove(this);
	m_Manager->RaiseDisconnect(this);
	delete this;
}

bool ServerPolledConnection::SendLine(const std::string *str)
{
	ScopedLock lock = ScopedLock(&m_WriteMutex);
	if (m_WriteBuffer.PushData(str->c_str(), str->size()) < 0)
		return false;
	lock.Unlock();
	m_poller->Update(this);
	return true;
}

