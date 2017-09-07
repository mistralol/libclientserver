
#include <libclientserver.h>

ServerTCP6PolledListener::ServerTCP6PolledListener(ServerTCPPolled *Server)
{
	m_Server = Server;
}

ServerTCP6PolledListener::~ServerTCP6PolledListener()
{
	if (close(m_fd) < 0)
		abort();
}

void ServerTCP6PolledListener::Init(int port, const std::string &address)
{
	m_port = port;
	m_addr = address;

	struct sockaddr_in6 addr;
	size_t addr_len = sizeof(addr);

	m_fd = socket(AF_INET6, SOCK_STREAM, 0);
	if (m_fd < 0) {
		std::string err = strerror(errno);
		throw(err);
	}

	int enable = 1;
	if (setsockopt(m_fd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0) {
		std::string err = "setsockopt(SO_REUSEADDR): " + std::string(strerror(errno));
		throw(std::runtime_error(err));
	}

	memset(&addr, 0, addr_len);

	addr.sin6_family = AF_INET6;
	if (inet_pton(AF_INET6, address.c_str(), &addr.sin6_addr) != 1) {
		throw(std::runtime_error("Cannot parse IPv6 address"));
	}
	addr.sin6_port = htons(port);

	if(bind(m_fd, (struct sockaddr *) &addr, addr_len) < 0)
	{
		std::string err = "bind: " + std::string(strerror(errno));
		if (close(m_fd) < 0)
			abort();
		throw(std::runtime_error(err));
	}
    
	if(listen(m_fd, 512) < 0) {
		std::string err = "listen: " + std::string(strerror(errno));
		if (close(m_fd) < 0)
			abort();
		throw(std::runtime_error(err));
	}
}

bool ServerTCP6PolledListener::CanRead(const Poller *)
{
	return true;
}

bool ServerTCP6PolledListener::CanWrite(const Poller *)
{
	return false;
}

bool ServerTCP6PolledListener::CanExcept(const Poller *)
{
	return false;
}

bool ServerTCP6PolledListener::CanTimeout(const Poller *)
{
	return false;
}

void ServerTCP6PolledListener::DoRead(Poller *sel)
{
	struct sockaddr_un addr;
	socklen_t addr_len = sizeof(addr);

	int fd = accept(m_fd, (struct sockaddr *) &addr, &addr_len);
	if (fd < 0)
		return;

	m_Server->CreateNewConnection(fd);
}

void ServerTCP6PolledListener::DoWrite(Poller *)
{
	//Should never be called
}

void ServerTCP6PolledListener::DoExcept(Poller *)
{
	//Should never be called
}

void ServerTCP6PolledListener::DoTimeout(Poller *)
{
	//Should never be called
}

void ServerTCP6PolledListener::DoClose(Poller *sel)
{
}

int ServerTCP6PolledListener::GetFD(const Poller *)
{
	return m_fd;
}

void ServerTCP6PolledListener::GetTimeout(const Poller *, struct timespec *tv)
{
	//Should never be called
}

