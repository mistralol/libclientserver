
class Process
{
	public:
		Process(const std::string exe);
		Process(const std::string exe, std::list<std::string> args);
		~Process();

		int Start();
		int Kill(int signum);
		bool HasExited();
		bool WaitforExit();
		int GetExitCode();

		void ArgsAdd(const std::string str);
		void ArgsClear();

	private:
		pid_t m_pid; //Child Process id
		bool m_started;
		bool m_alreadyexit;
		int m_exitcode;
		std::string m_exe;
		std::list<std::string> m_args;
};

