
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>

#include <libclientserver.h>


ClientUnix::ClientUnix(const std::string path)
{
	Init();
	m_fd = -1;
	m_path = path;
	m_connected = false;
}

ClientUnix::~ClientUnix()
{
	if (m_quit == false)
		Disconnect();
}

void ClientUnix::Connect()
{
	m_quit = false;
	Thread::Start();
}

bool ClientUnix::IsConnected()
{
	return m_connected;
}

void ClientUnix::Disconnect()
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

bool ClientUnix::SendLine(const std::string *str, const struct timespec *Timeout)
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

void ClientUnix::Run()
{
	struct sockaddr_un addr;
	size_t addr_len = sizeof(addr);

	while(m_quit == false)
	{
		Buffer buffer = Buffer();
		int fd = socket(AF_UNIX, SOCK_STREAM, 0);
		if (fd < 0)
		{
			RaiseOnConnectError(errno, Errno::ToStr());
			Time::Sleep(&m_ReConnectDelay);
			if (close(fd) < 0)
				abort();
			continue;
		}

		memset(&addr, 0, addr_len);

		addr.sun_family = AF_UNIX;
		snprintf(addr.sun_path, sizeof(addr.sun_path), "%s", m_path.c_str());

		if (connect(fd, (struct sockaddr *) &addr, addr_len) < 0)
		{
			RaiseOnConnectError(errno, Errno::ToStr());
			if (close(fd) < 0)
				abort();
			
			Time::Sleep(&m_ReConnectDelay);
			continue;
		}

		m_fd = fd;	//Allow write to access this now that we are connected
		RaiseOnConnect();

		//Read Responses / Events / Etc....
		//FIXME: Need converted to non-blocking or a dead connection / server stops the disconnect from working!
		while(m_quit == false && m_fd >= 0)
		{
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


