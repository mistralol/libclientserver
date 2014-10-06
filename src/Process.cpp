
#include <libclientserver.h>

Process::Process(const std::string exe)
{
	m_exe = exe;
	m_alreadyexit = false;
	m_exitcode = 0;
	m_started = false;
}

Process::Process(const std::string exe, std::list<std::string> args)
{
	m_exe = exe;
	m_args = args;
	m_alreadyexit = false;
	m_exitcode = 0;
	m_started = false;
}

Process::~Process()
{
	if (m_started == true)
		abort();
}

int Process::Start()
{
#ifdef DEBUG
	if (m_started == true)
		abort();
#endif
	const char **args = (const char **) malloc((m_args.size() + 2) * sizeof(*args));
	int i = 0;
	std::list<std::string>::iterator it = m_args.begin();
	args[i] = m_exe.c_str();
	i++;
	while(it != m_args.end())
	{
		args[i] = it->c_str();
		i++;
		it++;
	}
	args[i] = NULL;

	if (!args)
		abort();

	pid_t pid = fork();
	if (pid < 0)
	{
		free(args);
		return -errno;
	}

	if (pid > 0)
	{
		m_started = true;
		m_pid = pid;
		free(args);
		return pid;
	}

	//Actually start child process
	if (execv(m_exe.c_str(), ( char* const*) args) < 0)
	{
		exit(-errno << 8);
	}
	exit(-1); //Never Happens!
}

int Process::Kill(int signum)
{
	return kill(m_pid, signum);
}

bool Process::HasExited()
{
	int ret = waitpid(m_pid, &m_exitcode, WNOHANG);
	if (ret < 0)
	{
		switch(errno)
		{
			case ECHILD:
				return false;
			default:
				abort();
		}
	}

	return true;
}

bool Process::WaitforExit()
{
	if (m_alreadyexit == true)
		return true;

	int ret = waitpid(m_pid, &m_exitcode, 0);
	if (ret < 0)
	{
		switch(errno)
		{
			case ECHILD:
				return false;
			default:
				abort();
		}
	}
	m_started = false;
	m_alreadyexit = true;
	return true;
}

int Process::GetExitCode()
{
#ifdef DEBUG
	if (m_alreadyexit != true || m_started == true) //Invalid code because process is still running or not started
		abort();
#endif
	return WEXITSTATUS(m_exitcode);
}

void Process::ArgsAdd(const std::string str)
{
	m_args.push_back(str);
}

void Process::ArgsClear()
{
	m_args.clear();
}


