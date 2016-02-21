
#include <libclientserver.h>

class TcpConnection : public IPollable
{
	private:
		Buffer m_buffer;
		int m_fd;
		int m_null;
		bool m_delete;

	public:
		TcpConnection(int fd)
		{
			m_fd = fd;
			m_delete = false;
			
			m_null = open("/dev/null", O_WRONLY);
			if (m_null < 0)
			{
				perror("open(/dev/null");
				abort();
			}

			if (fcntl(fd, F_SETFL, O_NONBLOCK) < 0)
			{
				perror("fnctl");
				abort();
			}

			if (m_buffer.Init() < 0)
				abort();
		}
		
		~TcpConnection()
		{
			if (close(m_fd) < 0)
			{
				perror("close");
				abort();
			}
			
			if (close(m_null) < 0)
			{
				perror("close");
				abort();
			}
		}

		bool CanRead(const Poller *)
		{
			return true;
		}

		bool CanWrite(const Poller *)
		{
			return false;
		}		

		bool CanTimeout(const Poller *)
		{
			return true;
		}

		void DoRead(Poller *sel)
		{
			if (m_buffer.Read(m_fd) < 0)
			{
				sel->Remove(this);
				delete this;
			}
			
			if (m_buffer.Write(m_null) < 0)
			{
				printf("Failed to write to /dev/null %s\n", strerror(errno));
				sel->Remove(this);
				delete this;
			}
		}

		void DoWrite(Poller *sel)
		{

		}

		void DoTimeout(Poller *sel)
		{
			printf("Tcp Connection Timeout\n");
			sel->Remove(this);
			delete this;
		}

		void DoClose(Poller *sel)
		{
			sel->Remove(this);
			delete this;
		}

		int GetFD(const Poller *)
		{
			return m_fd;
		}

		void GetTimeout(const Poller *, struct timespec *tv)
		{
			tv->tv_sec = 30;
			tv->tv_nsec = 0;
		}

};

class Listen : public IPollable
{
	private:
		int m_fd;

	public:
		Listen(int port)
		{
			m_fd = socket(AF_INET, SOCK_STREAM, 0);
			if (m_fd < 0)
			{
				perror("socket");
				abort();
			}

			struct sockaddr_in addr;
			memset(&addr, 0, sizeof(addr));
			addr.sin_family = AF_INET;
			addr.sin_addr.s_addr = htonl(INADDR_ANY);
			addr.sin_port = htons(port);

			int opt = 1;
			if (setsockopt(m_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
			{
				perror("setsockopt");
				abort();
			}

			if (bind(m_fd, (struct sockaddr *) &addr, sizeof(addr)) < 0)
			{
				perror("bind");
				abort();
			}

			if (listen(m_fd, 1024) < 0)
			{
				perror("listen");
				abort();
			}
		}
		
		~Listen()
		{
			if (close(m_fd) < 0)
			{
				perror("close");
				abort();
			}
		}

		bool CanRead(const Poller *)
		{
			return true;
		}

		bool CanWrite(const Poller *)
		{
			return false;
		}		

		bool CanTimeout(const Poller *)
		{
			return false;
		}

		void DoRead(Poller *sel)
		{
			struct sockaddr_in caddr;
			socklen_t len = sizeof(caddr);
			int nfd = accept(m_fd, (struct sockaddr *) &caddr, &len);
			if (nfd < 0)
			{
				perror("accept");
				return;
			}

			TcpConnection *echo = new TcpConnection(nfd);
			sel->Add(echo);
		}

		void DoWrite(Poller *)
		{
			abort();
		}

		void DoTimeout(Poller *)
		{
			//printf("Listen Timeout\n");
		}

		void DoClose(Poller *)
		{

		}

		int GetFD(const Poller *)
		{
			return m_fd;
		}

		void GetTimeout(const Poller *, struct timespec *tv)
		{

		}
};


int main(int argc, char **argv)
{
	Poller pol = Poller();
	Listen listen = Listen(5454);

	pol.Add(&listen);

	printf("Now Listening on port 5454\n");
	
	while(1)
	{
		sleep(120);
	}

	pol.Remove(&listen);

	return 0;
}


