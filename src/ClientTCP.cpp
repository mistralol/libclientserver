
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>

#include <libclientserver.h>


ClientTCP::ClientTCP(int port, const std::string &addr) :
	m_fd(-1),
	m_port(port),
	m_addr(addr)
{
	Init();
	m_connected = false;
}

ClientTCP::~ClientTCP()
{
	if (m_quit == false)
		Disconnect();
}

void ClientTCP::Connect()
{
	m_quit = false;
	Thread::Start();
}

bool ClientTCP::IsConnected()
{
	return m_connected;
}

void ClientTCP::Disconnect()
{
	m_quit = true;
	ScopedReadLock rlock(&m_WriterLock);
	if (m_fd >= 0)
	{
		if (shutdown(m_fd, SHUT_WR) < 0)
			abort();	//FIXME: This can be a valid error case
	}
	rlock.Unlock();

	Thread::Stop();
}

bool ClientTCP::SendLine(const std::string *str, const struct timespec *Timeout)
{
	ScopedLock lock1(&m_WriterMutex);
	if (IsConnected() == false)
		return false;

	ScopedReadLock rlock(&m_WriterLock);

	const char *c = str->c_str();
	ssize_t offset = 0;
	ssize_t length = str->size();
	ssize_t ret = 0;
	
restart:
	ret = send(m_fd, &c[offset], length, MSG_NOSIGNAL);
	if (ret < 0)
	{
		switch(errno)
		{
			case EINTR:
				goto restart;
				break;
			default:
				RaiseOnSendError(errno, Errno::ToStr());
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
	return true;
}

int ClientTCP::Connect4(struct sockaddr_in &addr) {
	size_t addr_len = sizeof(addr);

	int fd = socket(AF_INET, SOCK_STREAM, 0);
	if (fd < 0)
	{
		RaiseOnConnectError(errno, Errno::ToStr());
		if (close(fd) < 0)
			abort();
		return -1;
	}


	if (connect(fd, (struct sockaddr *) &addr, addr_len) < 0)
	{
		RaiseOnConnectError(errno, Errno::ToStr());
		if (close(fd) < 0)
			abort();
		
		return -1;
	}
	return fd;
}

int ClientTCP::Connect6(struct sockaddr_in6 &addr) {
	size_t addr_len = sizeof(addr);

	int fd = socket(AF_INET6, SOCK_STREAM, 0);
	if (fd < 0)
	{
		RaiseOnConnectError(errno, Errno::ToStr());
		if (close(fd) < 0)
			abort();
		return -1;
	}

	if (connect(fd, (struct sockaddr *) &addr, addr_len) < 0)
	{
		RaiseOnConnectError(errno, Errno::ToStr());
		if (close(fd) < 0)
			abort();

		return -1;
	}
	return fd;
}


void ClientTCP::Run()
{

	while(m_quit == false)
	{
		struct sockaddr_in ipv4;
		struct sockaddr_in6 ipv6;

		memset(&ipv4, 0, sizeof(ipv4));
		ipv4.sin_family = AF_INET;
		ipv4.sin_port = htons(m_port);

		memset(&ipv6, 0, sizeof(ipv6));
		ipv6.sin6_family = AF_INET6;
		ipv6.sin6_port = htons(m_port);

		int fd = -1;
		if (inet_pton(AF_INET, m_addr.c_str(), &ipv4.sin_addr) == 1) {
			fd = Connect4(ipv4);
		} else if (inet_pton(AF_INET6, m_addr.c_str(), &ipv6.sin6_addr) == 1) {
			fd = Connect6(ipv6);
		} else {
			struct addrinfo hints;
			struct addrinfo *hosts;

			memset(&hints, 0, sizeof(struct addrinfo));
			hints.ai_family = AF_UNSPEC;
			hints.ai_socktype = SOCK_STREAM;
			hints.ai_flags = 0;
			hints.ai_protocol = 0;

			//Try each host in turn
			std::string port = std::to_string(m_port);
			int hret = getaddrinfo(m_addr.c_str(), port.c_str(), &hints, &hosts);
			if (hret == 0) {
				struct addrinfo *p = hosts;
				while(p) {
					fd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
					if (fd < 0) {
						RaiseOnConnectError(errno, Errno::ToStr());
						continue;
					}

					if (connect(fd, p->ai_addr, p->ai_addrlen) != -1) {
						break; //Success
					}

					if (close(fd) < 0)
						abort();
					fd = -1;
					p = p->ai_next;
				}
				freeaddrinfo(hosts);
			} else {
				RaiseOnConnectError(errno, Errno::ToStr());
			}
		}

		if (fd < 0) {
			//Host lookups can take *time* check we are not exiting again
			if (m_quit == false)
				Time::Sleep(&m_ReConnectDelay);
			continue;
		}

		m_fd = fd;	//Allow write to access this now that we are connected
		RaiseOnConnect();

		//Read Responses / Events / Etc....
		//FIXME: Need converted to non-blocking or a dead connection / server stops the disconnect from working!
		while(m_quit == false && m_fd >= 0)
		{	
			Buffer buffer = Buffer();
			int ret = buffer.Read(m_fd);
			if (ret <= 0)
			{
				RaiseOnDisconnect(ret, Errno::ToStr(ret));
				ScopedWriteLock wlock(&m_WriterLock);	//We need to protect m_fd from the writer side while we close it
				if (close(m_fd) < 0)
					abort();
				m_fd = -1;
				break;
			}

			//Read Line at a time
			std::string line;
			bool HasLine = buffer.GetLine(&line);
			while(HasLine == true)
			{
				RaiseOnData(&line);
				HasLine = buffer.GetLine(&line);
			}
		}
	} //m_quit == false
}


