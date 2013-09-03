
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>

#include <libclientserver.h>

ServerUnix::ServerUnix(const std::string &path)
{
	m_backlog = 32;
	m_path = path;
	m_fd = -1;
	m_quit = true;
}

ServerUnix::~ServerUnix()
{
	if (IsRunning())
		abort();	//Attempted to remove an active server
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

	//FIXME: Change File Permissions

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
	if (close(m_fd) < 0)
		abort();

	m_manager = NULL;
	Thread::Stop();
}

void ServerUnix::Run()
{
	while(m_quit == false)
	{
		struct sockaddr_un addr;
		socklen_t addr_len = sizeof(addr);
		int fd = accept(m_fd, (struct sockaddr *) &addr, &addr_len);
		if (fd < 0) {
			std::string err = strerror(errno);
			//Logger("Server::Unix::Run() -> accept: %s", err.c_str());
			continue;
		}

		ServerUnixConnection *Connection = new ServerUnixConnection(m_manager, this, fd);
		Connection->Start();
	}
}

