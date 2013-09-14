
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>

#include <libclientserver.h>


ClientUnix::ClientUnix(const std::string path)
{
	Init();
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

bool ClientUnix::IsConnected()
{
	return m_connected;
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
		ReadBuffer Buffer(1024 * 2048);
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
			RaiseOnConnectError(errno, Errno::ToStr());
			if (close(m_fd) < 0)
				abort();
			m_fd = -1;
			
			Time::Sleep(&m_ReConnectTimeout);
			continue;
		}

		m_connected = true;
		RaiseOnConnect();

		//Read Responses / Events / Etc....
		while(m_quit == false && m_fd >= 0)
		{
			int ret = Buffer.Read(m_fd);
			if (ret <= 0)
			{
				RaiseOnDisconnect(ret, Errno::ToStr(ret));
				if (close(m_fd) < 0)
					abort();
				m_fd = -1;
			}

			bool Found = false;
			do { //There can be more that one request per read!
				char *tmp = Buffer.GetStartPtr();
				size_t pos = Buffer.GetPosition();
			
				//Find End Marker
				char *lf = (char *) memchr(tmp, '\n', pos);
				if (lf == NULL)
				{
					Found = false;
					break;
				}
				else
				{
					Found = true;
				}
	
				char *space = (char *) memchr(tmp, ' ', (size_t) (lf - pos));
				if (space == NULL) //Protocol Error Bail!
				{
					RaiseOnDisconnect(ret, Errno::ToStr(ret));
					if (close(m_fd) < 0)
						abort();
					m_fd = -1;
					break;
				}
				*space = 0;
				printf("%s\n", tmp);
				abort(); //Parsing Is Required!
			
				Buffer.Shift((size_t) (lf - pos));
			} while(Found);
		}

		m_connected = false;
		//RaiseOnDisconnect();
	}
}


