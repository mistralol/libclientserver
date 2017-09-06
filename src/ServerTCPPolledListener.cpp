
#include <libclientserver.h>

ServerTCPPolledListener::ServerTCPPolledListener(ServerTCPPolled *Server)
{
	m_Server = Server;
}

ServerTCPPolledListener::~ServerTCPPolledListener()
{
	if (close(m_fd) < 0)
		abort();
}

void ServerTCPPolledListener::Init(int port, const std::string &address)
{
	m_port = port;
	m_addr = address;

	struct sockaddr_in addr;
	size_t addr_len = sizeof(addr);

	m_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (m_fd < 0) {
		std::string err = strerror(errno);
		throw(err);
	}

	memset(&addr, 0, addr_len);

	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr(address.c_str());
	addr.sin_port = htons(port);

	if(bind(m_fd, (struct sockaddr *) &addr, addr_len) < 0)
	{
		std::string err = strerror(errno);
		if (close(m_fd) < 0)
			abort();
		throw(err);
	}
    
	if(listen(m_fd, 512) < 0) {
		std::string err = strerror(errno);
		if (close(m_fd) < 0)
			abort();
		throw(err);
	}
}

bool ServerTCPPolledListener::CanRead(const Poller *)
{
	return true;
}

bool ServerTCPPolledListener::CanWrite(const Poller *)
{
	return false;
}

bool ServerTCPPolledListener::CanExcept(const Poller *)
{
	return false;
}

bool ServerTCPPolledListener::CanTimeout(const Poller *)
{
	return false;
}

void ServerTCPPolledListener::DoRead(Poller *sel)
{
	struct sockaddr_un addr;
	socklen_t addr_len = sizeof(addr);

	int fd = accept(m_fd, (struct sockaddr *) &addr, &addr_len);
	if (fd < 0)
		return;

	m_Server->CreateNewConnection(fd);
}

void ServerTCPPolledListener::DoWrite(Poller *)
{
	//Should never be called
}

void ServerTCPPolledListener::DoExcept(Poller *)
{
	//Should never be called
}

void ServerTCPPolledListener::DoTimeout(Poller *)
{
	//Should never be called
}

void ServerTCPPolledListener::DoClose(Poller *sel)
{
}

int ServerTCPPolledListener::GetFD(const Poller *)
{
	return m_fd;
}

void ServerTCPPolledListener::GetTimeout(const Poller *, struct timespec *tv)
{
	//Should never be called
}

