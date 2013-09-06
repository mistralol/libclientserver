
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>

#include <libclientserver.h>


ClientUnix::ClientUnix(const std::string path)
{
	m_fd = -1;
	m_path = path;
}


ClientUnix::~ClientUnix()
{
	Disconnect();
}

void ClientUnix::Connect()
{
	m_quit = false;
	Thread::Start();
}

void ClientUnix::Disconnect()
{
	m_quit = true;
	Thread::Stop();
}

void ClientUnix::DoSendRequest(Request *request, Request *response)
{
	abort();
}

void ClientUnix::DoSendCommand(Request *request)
{
	abort();
}

void ClientUnix::Run()
{
	struct sockaddr_un addr;
	size_t addr_len = sizeof(addr);

	while(m_quit == false)
	{
		m_fd = socket(AF_UNIX, SOCK_STREAM, 0);
		if (m_fd < 0)
		{
			abort();
		}

		memset(&addr, addr_len, 0);

		addr.sun_family = AF_UNIX;
		snprintf(addr.sun_path, sizeof(addr.sun_path), "%s", m_path.c_str());

		if (connect(m_fd, (struct sockaddr *) &addr, addr_len) < 0)
		{
			if (close(m_fd) < 0)
				abort();
			m_fd = -1;
		}

		//Write Data!!

		abort();
	}
}


