
#include <libclientserver.h>

ServerSelectedConnection::ServerSelectedConnection(ServerManager *Manager, IServer *Server, Selector *sel, int fd)
{
	m_Manager = Manager;
	m_Server = Server;
	m_selector = sel;
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

ServerSelectedConnection::~ServerSelectedConnection()
{
	if (close(m_fd) < 0)
		abort();
}

bool ServerSelectedConnection::CanRead(const Selector *)
{
	return true;
}

bool ServerSelectedConnection::CanWrite(const Selector *)
{
	ScopedLock lock = ScopedLock(&m_WriteMutex);
	if (m_WriteBuffer.GetDataLength() > 0)
		return true;
	return false;
}

bool ServerSelectedConnection::CanExcept(const Selector *)
{
	return false;
}

bool ServerSelectedConnection::CanTimeout(const Selector *)
{
	return true;
}

void ServerSelectedConnection::DoRead(Selector *sel)
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

void ServerSelectedConnection::DoWrite(Selector *sel)
{
	ScopedLock lock = ScopedLock(&m_WriteMutex);
	if (m_WriteBuffer.Write(m_fd) < 0)
	{
		Destroy(sel);
		return;
	}
}

void ServerSelectedConnection::DoExcept(Selector *)
{

}

void ServerSelectedConnection::DoTimeout(Selector *sel)
{
	Destroy(sel);
}

void ServerSelectedConnection::DoClose(Selector *sel)
{
	Destroy(sel);
}

int ServerSelectedConnection::GetFD(const Selector *)
{
	return m_fd;
}

void ServerSelectedConnection::GetTimeout(const Selector *, struct timespec *tv)
{
	tv->tv_sec = 300;
	tv->tv_nsec = 0;
}

void ServerSelectedConnection::Destroy(Selector *sel)
{
	sel->Remove(this);
	m_Server->ConnectionRemove(this);
	m_Manager->RaiseDisconnect(this);
	delete this;
}

bool ServerSelectedConnection::SendLine(const std::string *str)
{
	ScopedLock lock = ScopedLock(&m_WriteMutex);
	if (m_WriteBuffer.PushData(str->c_str(), str->size()) < 0)
		return false;
	lock.Unlock();
	m_selector->Update(this);
	return true;
}

