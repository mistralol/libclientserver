
class ServerUnixPolledListener : public IPollable
{
	public:
		ServerUnixPolledListener(ServerUnixPolled *Server);
		~ServerUnixPolledListener();

		void Init(const std::string &path, mode_t perms);

		bool CanRead(const Poller *);
		bool CanWrite(const Poller *);
		bool CanExcept(const Poller *);
		bool CanTimeout(const Poller *);

		void DoRead(Poller *sel);
		void DoWrite(Poller *);
		void DoExcept(Poller *);
		void DoTimeout(Poller *);
		void DoClose(Poller *);

		int GetFD(const Poller *);

		void GetTimeout(const Poller *, struct timespec *tv);

	private:
		ServerUnixPolled *m_Server;
		std::string m_path;
		mode_t m_perm;
		int m_fd;
};

