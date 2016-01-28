
#include <libclientserver.h>

ServerUnixPolledListener::ServerUnixPolledListener(ServerUnixPolled *Server)
{
	m_Server = Server;
}

ServerUnixPolledListener::~ServerUnixPolledListener()
{
	if (close(m_fd) < 0)
		abort();
}

void ServerUnixPolledListener::Init(const std::string &path, mode_t perm)
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

bool ServerUnixPolledListener::CanRead(const Poller *)
{
	return true;
}

bool ServerUnixPolledListener::CanWrite(const Poller *)
{
	return false;
}

bool ServerUnixPolledListener::CanExcept(const Poller *)
{
	return false;
}

bool ServerUnixPolledListener::CanTimeout(const Poller *)
{
	return false;
}

void ServerUnixPolledListener::DoRead(Poller *sel)
{
	struct sockaddr_un addr;
	socklen_t addr_len = sizeof(addr);

	int fd = accept(m_fd, (struct sockaddr *) &addr, &addr_len);
	if (fd < 0)
		return;

	m_Server->CreateNewConnection(fd);
}

void ServerUnixPolledListener::DoWrite(Poller *)
{
	abort(); //Should never be called
}

void ServerUnixPolledListener::DoExcept(Poller *)
{
	abort(); //Should never be called
}

void ServerUnixPolledListener::DoTimeout(Poller *)
{
	abort(); //Should never be called
}

void ServerUnixPolledListener::DoClose(Poller *sel)
{
}

int ServerUnixPolledListener::GetFD(const Poller *)
{
	return m_fd;
}

void ServerUnixPolledListener::GetTimeout(const Poller *, struct timespec *tv)
{
	abort(); //Should never be called
}

