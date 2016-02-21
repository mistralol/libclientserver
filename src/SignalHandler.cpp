
#include <libclientserver.h>

SignalHandler::SignalHandler(ISignalHandler *handler)
{
	m_handler = handler;
	m_loop = true;

	if (sigemptyset(&m_sigs) < 0)
		abort();

	if (sigaddset(&m_sigs, SIGALRM) < 0)
		abort();

	if (sigaddset(&m_sigs, SIGCHLD) < 0)
		abort();

	if (sigaddset(&m_sigs, SIGHUP) < 0)
		abort();

	if (sigaddset(&m_sigs, SIGTERM) < 0)
		abort();

	if (sigaddset(&m_sigs, SIGUSR1) < 0)
		abort();

	if (sigaddset(&m_sigs, SIGUSR2) < 0)
		abort();

	if (sigaddset(&m_sigs, SIGPIPE) < 0)
		abort();


	if (sigprocmask(SIG_BLOCK, &m_sigs, NULL) < 0)
	{
		abort();
	}

	Thread::Start();
}

SignalHandler::~SignalHandler()
{
	m_loop = false;
	Thread::Signal(SIGUSR1);
	Thread::Stop();

	if (sigprocmask(SIG_UNBLOCK, &m_sigs, NULL) < 0)
	{
		abort();
	}
}

void SignalHandler::Block()
{
	m_mutex.Lock();
}

void SignalHandler::UnBlock()
{
	m_mutex.Unlock();
}

void SignalHandler::Run()
{
	while(1)
	{
		siginfo_t info;
		if (sigwaitinfo(&m_sigs, &info) < 0)
		{
			switch(errno)
			{
				case EINTR:
					continue;
					break;
				default:
					abort();
			}
		}

		if (m_loop == false)
			break;

		m_mutex.Lock();
		switch(info.si_signo)
		{
			case SIGALRM:
				m_handler->SigAlarm(&info);
				break;
			case SIGCHLD:
				m_handler->SigChild(&info);
				break;
			case SIGHUP:
				m_handler->SigHUP(&info);
				break;
			case SIGTERM:
				m_handler->SigTerm(&info);
				break;
			case SIGUSR1:
				m_handler->SigUser1(&info);
				break;
			case SIGUSR2:
				m_handler->SigUser2(&info);
				break;
			case SIGPIPE:
				m_handler->SigPipe(&info);
				break;
			default:
				abort(); //Unknown signal
				break;
		}
		m_mutex.Unlock();
	}
}

