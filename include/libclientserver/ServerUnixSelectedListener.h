
class ServerUnixSelectedListener : public ISelectable
{
	public:
		ServerUnixSelectedListener(ServerUnixSelected *Server);
		~ServerUnixSelectedListener();

		void Init(const std::string &path, mode_t perms);

		bool CanRead(const Selector *);
		bool CanWrite(const Selector *);
		bool CanExcept(const Selector *);
		bool CanTimeout(const Selector *);

		void DoRead(Selector *sel);
		void DoWrite(Selector *);
		void DoExcept(Selector *);
		void DoTimeout(Selector *);
		void DoClose(Selector *);

		int GetFD(const Selector *);

		void GetTimeout(const Selector *, struct timespec *tv);

	private:
		ServerUnixSelected *m_Server;
		std::string m_path;
		mode_t m_perm;
		int m_fd;
};

