
#include <libclientserver.h>

class TcpEcho : public ISelectable
{
	private:
		Buffer m_buffer;
		int m_fd;
		bool m_delete;

	public:
		TcpEcho() { }
		~TcpEcho()
		{
			close(m_fd);
		}

		void Init(int fd)
		{
			m_fd = fd;
			m_delete = false;

			if (fcntl(fd, F_SETFL, O_NONBLOCK) < 0)
			{
				perror("fnctl");
				abort();
			}

			if (m_buffer.Init() < 0)
				abort();
		}

		bool CanRead(const Selector *)
		{
			return true;
		}

		bool CanWrite(const Selector *)
		{
			if (m_buffer.GetDataLength() > 0)
				return true;
			return false;
		}		

		bool CanExcept(const Selector *)
		{
			return false;
		}

		bool CanTimeout(const Selector *)
		{
			return true;
		}

		void DoRead(Selector *sel)
		{
			if (m_buffer.Read(m_fd) < 0)
			{
				sel->Remove(this);
				delete this;
			}
		}

		void DoWrite(Selector *sel)
		{
			if (m_buffer.Write(m_fd) < 0)
			{
				sel->Remove(this);
				delete this;
			}
		}

		void DoExcept(Selector *)
		{
			abort();
		}

		void DoTimeout(Selector *sel)
		{
			printf("Tcp Echo Timeout\n");
			sel->Remove(this);
			delete this;
		}

		void DoClose(Selector *sel)
		{
			sel->Remove(this);
			delete this;
		}

		int GetFD(const Selector *)
		{
			return m_fd;
		}

		void GetTimeout(const Selector *, struct timespec *tv)
		{
			tv->tv_sec = 8;
			tv->tv_nsec = 0;
		}

};

class Listen : public ISelectable
{
	private:
		int m_fd;

	public:
		Listen() { }
		~Listen()
		{
			close(m_fd);
		}

		void Init()
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
			addr.sin_port = htons(3133);

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

		bool CanRead(const Selector *)
		{
			return true;
		}

		bool CanWrite(const Selector *)
		{
			return false;
		}		

		bool CanExcept(const Selector *)
		{
			return false;
		}

		bool CanTimeout(const Selector *)
		{
			return true;
		}

		void DoRead(Selector *sel)
		{
			struct sockaddr_in caddr;
			socklen_t len = sizeof(caddr);
			int nfd = accept(m_fd, (struct sockaddr *) &caddr, &len);
			if (accept < 0)
			{
				perror("accept");
				return;
			}

			TcpEcho *echo = new TcpEcho();
			echo->Init(nfd);
			sel->Add(echo);
		}

		void DoWrite(Selector *)
		{
			abort();
		}

		void DoExcept(Selector *)
		{
			abort();
		}

		void DoTimeout(Selector *)
		{
			//printf("Listen Timeout\n");
		}

		void DoClose(Selector *sel)
		{

		}

		int GetFD(const Selector *)
		{
			return m_fd;
		}

		void GetTimeout(const Selector *, struct timespec *tv)
		{
			tv->tv_sec = 5;
			tv->tv_nsec = 0;
		}
};


int main(int argc, char **argv)
{
	Selector *sel = new Selector();
	Listen *listen = new Listen();

	listen->Init();
	sel->Add(listen);
	sleep(10);

	sel->Remove(listen);
	delete listen;
	delete sel;

	return 0;
}


