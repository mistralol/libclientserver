
#include <libclientserver.h>

ServerTCPSelectedListener::ServerTCPSelectedListener(ServerTCPSelected *Server)
{
	m_Server = Server;
}

ServerTCPSelectedListener::~ServerTCPSelectedListener()
{
	if (close(m_fd) < 0)
		abort();
}

void ServerTCPSelectedListener::Init(int port, const std::string &address)
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

	int enable = 1;
	if (setsockopt(m_fd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0) {
		std::string err = "setsockopt(SO_REUSEADDR): " + std::string(strerror(errno));
		throw(std::runtime_error(err));
	}

	memset(&addr, 0, addr_len);

	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr(address.c_str());
	addr.sin_port = htons(port);

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

bool ServerTCPSelectedListener::CanRead(const Selector *)
{
	return true;
}

bool ServerTCPSelectedListener::CanWrite(const Selector *)
{
	return false;
}

bool ServerTCPSelectedListener::CanExcept(const Selector *)
{
	return false;
}

bool ServerTCPSelectedListener::CanTimeout(const Selector *)
{
	return false;
}

void ServerTCPSelectedListener::DoRead(Selector *sel)
{
	struct sockaddr_in addr;
	socklen_t addr_len = sizeof(addr);

	int fd = accept(m_fd, (struct sockaddr *) &addr, &addr_len);
	if (fd < 0)
		return;

	m_Server->CreateNewConnection(fd);
}

void ServerTCPSelectedListener::DoWrite(Selector *)
{
	//Should never be called
}

void ServerTCPSelectedListener::DoExcept(Selector *)
{
	//Should never be called
}

void ServerTCPSelectedListener::DoTimeout(Selector *)
{
	//Should never be called
}

void ServerTCPSelectedListener::DoClose(Selector *sel)
{
}

int ServerTCPSelectedListener::GetFD(const Selector *)
{
	return m_fd;
}

void ServerTCPSelectedListener::GetTimeout(const Selector *, struct timespec *tv)
{
	//Should never be called
}

