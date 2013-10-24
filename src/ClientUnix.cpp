
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

bool ClientUnix::DoSendRequest(Request *request, const struct timespec *Timeout)
{
	std::string str = "REQUEST " + request->Encode() + "\n";
	return SendLine(&str, Timeout);
}

bool ClientUnix::DoSendCommand(Request *request, const struct timespec *Timeout)
{
	std::string str = "COMMAND " + request->Encode() + "\n";
	return SendLine(&str, Timeout);
}

bool ClientUnix::SendLine(const std::string *str, const struct timespec *Timeout)
{
	ScopedLock lock1(&m_WriterMutex);
	if (IsConnected() == false)
		return false;

	ScopedReadLock rlock(&m_WriterLock);
	printf("Writing(%d): %s\n", m_fd, str->c_str());
	int err = write(m_fd, str->c_str(), str->size());
	//FIXME: Deal with partial write
	if (err == (int) str->length())
		return true;

	RaiseOnSendError(errno, Errno::ToStr());
	return false;
}

void ClientUnix::Run()
{
	struct sockaddr_un addr;
	size_t addr_len = sizeof(addr);

	while(m_quit == false)
	{
		ReadBuffer Buffer(1024 * 2048);
		int fd = socket(AF_UNIX, SOCK_STREAM, 0);
		if (fd < 0)
		{
			RaiseOnConnectError(errno, Errno::ToStr());
			Time::Sleep(&m_ReConnectTimeout);
			if (close(fd) < 0)
				abort();
			continue;
		}

		memset(&addr, addr_len, 0);

		addr.sun_family = AF_UNIX;
		snprintf(addr.sun_path, sizeof(addr.sun_path), "%s", m_path.c_str());

		if (connect(fd, (struct sockaddr *) &addr, addr_len) < 0)
		{
			RaiseOnConnectError(errno, Errno::ToStr());
			if (close(fd) < 0)
				abort();
			
			Time::Sleep(&m_ReConnectTimeout);
			continue;
		}

		m_fd = fd;	//Allow write to access this now that we are connected
		m_connected = true;
		RaiseOnConnect();

		//Read Responses / Events / Etc....
		//FIXME: Need converted to non-blocking or a dead connection / server stops the disconnect from working!
		while(m_quit == false && m_fd >= 0)
		{
			int ret = Buffer.Read(m_fd);
			if (ret <= 0)
			{
				m_connected = false;
				RaiseOnDisconnect(ret, Errno::ToStr(ret));
				ScopedWriteLock wlock(&m_WriterLock);	//We need to protect m_fd from the writer side while we close it
				if (close(m_fd) < 0)
					abort();
				m_fd = -1;
				break;
			}

			//Read Line at a time
			std::string line;
			bool HasLine = Buffer.GetLine(&line);
			while(HasLine == true)
			{
				RaiseOnData(&line);
				HasLine = Buffer.GetLine(&line);
			}
		}
	} //m_quit == false
}


