
#include <libclientserver.h>

ServerUnixSelectedListener::ServerUnixSelectedListener(ServerUnixSelected *Server)
{
	m_Server = Server;
}

ServerUnixSelectedListener::~ServerUnixSelectedListener()
{
	if (close(m_fd) < 0)
		abort();
}

void ServerUnixSelectedListener::Init(const std::string &path, mode_t perm)
{
	m_path = path;
	m_perm = perm;
	struct sockaddr_un addr;
	size_t addr_len = sizeof(addr);

	unlink(m_path.c_str());
	m_fd = socket(AF_UNIX, SOCK_STREAM, 0);
	if (m_fd < 0) {
		std::string err = strerror(errno);
		throw(err);
	}

	memset(&addr, 0, addr_len);

	addr.sun_family = AF_UNIX;
	snprintf(addr.sun_path, sizeof(addr.sun_path), "%s", m_path.c_str());

	if(bind(m_fd, (struct sockaddr *) &addr, addr_len) < 0)
	{
		std::string err = strerror(errno);
		if (close(m_fd) < 0)
			abort();
		throw(err);
	}
    
	if (m_perm != 0)
	{
		if (chmod(m_path.c_str(), m_perm) < 0)
		{
			std::string err = strerror(errno);
			throw(err);
		}
	}

	if(listen(m_fd, 512) < 0) {
		std::string err = strerror(errno);
		if (close(m_fd) < 0)
			abort();
		throw(err);
	}

}

bool ServerUnixSelectedListener::CanRead(const Selector *)
{
	return true;
}

bool ServerUnixSelectedListener::CanWrite(const Selector *)
{
	return false;
}

bool ServerUnixSelectedListener::CanExcept(const Selector *)
{
	return false;
}

bool ServerUnixSelectedListener::CanTimeout(const Selector *)
{
	return false;
}

void ServerUnixSelectedListener::DoRead(Selector *sel)
{
	struct sockaddr_un addr;
	socklen_t addr_len = sizeof(addr);

	int fd = accept(m_fd, (struct sockaddr *) &addr, &addr_len);
	if (fd < 0)
		return;

	m_Server->CreateNewConnection(fd);
}

void ServerUnixSelectedListener::DoWrite(Selector *)
{
	abort(); //Should never be called
}

void ServerUnixSelectedListener::DoExcept(Selector *)
{
	abort(); //Should never be called
}

void ServerUnixSelectedListener::DoTimeout(Selector *)
{
	abort(); //Should never be called
}

void ServerUnixSelectedListener::DoClose(Selector *sel)
{
}

int ServerUnixSelectedListener::GetFD(const Selector *)
{
	return m_fd;
}

void ServerUnixSelectedListener::GetTimeout(const Selector *, struct timespec *tv)
{
	abort(); //Should never be called
}

