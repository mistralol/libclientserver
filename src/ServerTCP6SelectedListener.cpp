
#include <libclientserver.h>

ServerTCP6SelectedListener::ServerTCP6SelectedListener(ServerTCPSelected *Server)
{
	m_Server = Server;
}

ServerTCP6SelectedListener::~ServerTCP6SelectedListener()
{
	if (close(m_fd) < 0)
		abort();
}

void ServerTCP6SelectedListener::Init(int port, const std::string &address)
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

bool ServerTCP6SelectedListener::CanRead(const Selector *)
{
	return true;
}

bool ServerTCP6SelectedListener::CanWrite(const Selector *)
{
	return false;
}

bool ServerTCP6SelectedListener::CanExcept(const Selector *)
{
	return false;
}

bool ServerTCP6SelectedListener::CanTimeout(const Selector *)
{
	return false;
}

void ServerTCP6SelectedListener::DoRead(Selector *sel)
{
	struct sockaddr_in6 addr;
	socklen_t addr_len = sizeof(addr);

	int fd = accept(m_fd, (struct sockaddr *) &addr, &addr_len);
	if (fd < 0)
		return;

	m_Server->CreateNewConnection(fd);
}

void ServerTCP6SelectedListener::DoWrite(Selector *)
{
	//Should never be called
}

void ServerTCP6SelectedListener::DoExcept(Selector *)
{
	//Should never be called
}

void ServerTCP6SelectedListener::DoTimeout(Selector *)
{
	//Should never be called
}

void ServerTCP6SelectedListener::DoClose(Selector *sel)
{
}

int ServerTCP6SelectedListener::GetFD(const Selector *)
{
	return m_fd;
}

void ServerTCP6SelectedListener::GetTimeout(const Selector *, struct timespec *tv)
{
	//Should never be called
}

