
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>

#include <libclientserver.h>

ServerUnix::ServerUnix(const std::string &path)
{
	Init();
	m_path = path;
}

ServerUnix::ServerUnix(const std::string &path, mode_t perms)
{
	Init();
	m_path = path;
	m_perms = perms;
}

ServerUnix::~ServerUnix()
{
	if (IsRunning())
		abort();	//Attempted to remove an active server
}

void ServerUnix::Init()
{
	m_backlog = 32;
	m_perms = S_IRUSR | S_IWUSR;
	m_fd = -1;
	m_quit = true;
}

void ServerUnix::Start(ServerManager *Manager)
{
    struct sockaddr_un addr;
    size_t addr_len = sizeof(addr);

    unlink(m_path.c_str());
    m_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (m_fd < 0) {
        std::string err = strerror(errno);
        //Logger("ServerUnix::Start() -> socket: %s", strerror(errno));
        throw(err);
    }

    memset(&addr, addr_len, 0);

    addr.sun_family = AF_UNIX;
    snprintf(addr.sun_path, sizeof(addr.sun_path), "%s", m_path.c_str());

    if(bind(m_fd, (struct sockaddr *) &addr, addr_len) < 0) {
        std::string err = strerror(errno);
        //Logger("ServerUnix::Start() -> bind: %s", strerror(errno));
        if (close(m_fd) < 0)
			abort();
        throw(err);
    }
    
    if (chmod(m_path.c_str(), m_perms) < 0)
	{
        std::string err = strerror(errno);
		throw(err);
	}

    if(listen(m_fd, m_backlog) < 0) {
        std::string err = strerror(errno);
        //Logger("ServerUnix::Start() -> listen: %s", strerror(errno));
        if (close(m_fd) < 0)
			abort();
        throw(err);
    }

	m_manager = Manager;
	m_quit = false;
	Thread::Start();
}

void ServerUnix::Stop()
{
	m_quit = true;

	m_manager = NULL;
	Thread::Stop();
}

void ServerUnix::Run()
{
	while(m_quit == false)
	{
		fd_set fds;
		struct sockaddr_un addr;
		struct timeval tv;
		socklen_t addr_len = sizeof(addr);

		tv.tv_sec = 1;
		tv.tv_usec = 0;

		FD_ZERO(&fds);
		FD_SET(m_fd, &fds);
		int ret = select(m_fd + 1, &fds, NULL, NULL, &tv);
		if (ret < 0)
		{
			switch(errno)
			{
				case EINTR:
				case ETIMEDOUT:
					continue;
					break;
				default:
					abort();
			}
		}

		if (FD_ISSET(m_fd, &fds))
		{
			int fd = accept(m_fd, (struct sockaddr *) &addr, &addr_len);
			if (fd < 0)
				continue;

			m_manager->RaisePreNewConnection();

			ServerUnixConnection *Connection = new ServerUnixConnection(m_manager, this, fd);
			Connection->Start();
		}
	}

	if (close(m_fd) < 0)
		abort();
}

