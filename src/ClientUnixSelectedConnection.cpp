
#include <libclientserver.h>

ClientUnixSelectedConnection::ClientUnixSelectedConnection(ClientUnixSelected *client, const std::string &path)
{
	m_client = client;
	m_path = path;
	m_connected = false;
	m_failing = true;
	m_first = true;
	m_fd = -1;
	
	if (m_rbuffer.Init() < 0)
		abort();
		
	if (m_wbuffer.Init() < 0)
		abort();
		
	//Just give the selector a dead but valid socket that does nothing
	m_fd = socket(AF_UNIX, SOCK_STREAM, 0);
	if (m_fd < 0)
		abort();
	
	if (fcntl(m_fd, F_SETFL, O_NONBLOCK) < 0)
		abort();
}

ClientUnixSelectedConnection::~ClientUnixSelectedConnection()
{
	if (m_fd >= 0)
	{
		if (close(m_fd) < 0)
			abort();
	}
}

bool ClientUnixSelectedConnection::CanRead(const Selector *p)
{
	if (m_connected == false)
	{
		if (m_failing)
			return false;
		return true;
	}
	return true;
}

bool ClientUnixSelectedConnection::CanWrite(const Selector *p)
{
	if (m_connected == false)
		return m_connected;

	if (m_wbuffer.GetDataLength() > 0)
		return true;
	return false;
}

bool ClientUnixSelectedConnection::CanExcept(const Selector *p)
{
	return false;
}

bool ClientUnixSelectedConnection::CanTimeout(const Selector *p)
{
	return true;
}

void ClientUnixSelectedConnection::DoRead(Selector *p)
{
	if (m_connected == false)
	{
		return;
	}

	int ret = m_rbuffer.Read(m_fd);
	if (ret < 0)
	{
		m_connected = false;
		m_client->DoRaiseOnDisconnect(errno, Errno::ToStr());
		NewSocket(p);
		DoConnect();
	}
	
	//Read Line at a time
	std::string line;
	bool HasLine = m_rbuffer.GetLine(&line);
	while(HasLine == true)
	{
		m_client->DoRaiseOnData(&line);
		HasLine = m_rbuffer.GetLine(&line);
	}
}

void ClientUnixSelectedConnection::DoWrite(Selector *p)
{
	if (m_connected == false)
	{
		return;
	}

	int ret = m_wbuffer.Write(m_fd);
	if (ret < 0)
	{
		m_connected = false;
		m_client->DoRaiseOnDisconnect(errno, Errno::ToStr());
		NewSocket(p);
		DoConnect();
	}
}

void ClientUnixSelectedConnection::DoExcept(Selector *p)
{
	abort(); //Should never be called
}

void ClientUnixSelectedConnection::DoTimeout(Selector *p)
{
	if (m_failing == true)
	{
		NewSocket(p);
		DoConnect();
	}
	
	if (m_connected == false)
	{
		m_failing = true;
	}
}

void ClientUnixSelectedConnection::DoClose(Selector *p)
{
	m_connected = false;
}

int ClientUnixSelectedConnection::GetFD(const Selector *p)
{
	if (m_fd < 0)
		abort();
	return m_fd;
}

void ClientUnixSelectedConnection::GetTimeout(const Selector *, struct timespec *tv)
{
	if (m_connected == false)
	{
		if (m_first)
		{
			tv->tv_sec = 0;
			tv->tv_nsec = 1;
			m_first = false;
			return;
		}
		m_client->GetReconnectDelay(tv);
	}
	else
	{
		tv->tv_sec = 60;
		tv->tv_nsec = 0;
	}
}

bool ClientUnixSelectedConnection::SendLine(Selector *p, const std::string *str)
{
	if (m_connected == false)
		return false;
	m_wbuffer.PushData(str->c_str(), str->size());
	p->Update(this);
	return true;
}

void ClientUnixSelectedConnection::NewSocket(Selector *p)
{
	p->Remove(this);
	
	m_rbuffer.Clear();
	m_wbuffer.Clear();
	
	if (m_fd >= 0)
	{
		if (close(m_fd) < 0)
			abort();
		m_fd = -1;
	}
	
	m_fd = socket(AF_UNIX, SOCK_STREAM, 0);
	if (m_fd < 0)
		abort();
	
	if (fcntl(m_fd, F_SETFL, O_NONBLOCK) < 0)
		abort();
		
	p->Add(this);
}

void ClientUnixSelectedConnection::DoConnect()
{
	struct sockaddr_un addr;
	size_t addr_len = sizeof(addr);
	
	memset(&addr, 0, addr_len);

	addr.sun_family = AF_UNIX;
	snprintf(addr.sun_path, sizeof(addr.sun_path), "%s", m_path.c_str());

	int ret = connect(m_fd, (struct sockaddr *) &addr, addr_len);
	if (ret == 0)
	{
		m_failing = false;
		m_connected = true;
		m_client->DoRaiseOnConnect();
	}
	else
	{
		switch(ret)
		{
			case EINPROGRESS:
				return;
				break;
			default:
				m_client->DoRaiseOnConnectError(errno, Errno::ToStr());
				m_failing = true;
				break;
		}
	}
}

bool ClientUnixSelectedConnection::IsConnected()
{
	if (m_connected)
		return true;
	return false;
}



