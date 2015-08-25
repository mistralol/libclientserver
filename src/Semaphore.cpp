
#include <libclientserver.h>

Semaphore::Semaphore()
{
	if (sem_init(&m_sem, 0, 0) != 0)
		abort();
}

Semaphore::~Semaphore()
{
	if (sem_destroy(&m_sem) != 0)
		abort();
}

void Semaphore::Up()
{
	if (sem_post(&m_sem) != 0)
		abort();
}

void Semaphore::Down()
{
restart:
	int ret = sem_wait(&m_sem);
	if (ret < 0)
	{
		switch(errno)
		{
			case EINTR:
				goto restart;
				break;
			default:
				abort();
		}
	}
}

int Semaphore::TryDown()
{
restart:
	int ret = sem_trywait(&m_sem);
	if (ret < 0)
	{
		switch(errno)
		{
			case EINTR:
				goto restart;
				break;
			case EAGAIN:
				return -EAGAIN;
				break;
			default:
				abort();
		}
	}
	return 0;
}

int Semaphore::DownTimeout(const struct timespec *ts)
{
	struct timespec tabs;
	Time::MonoTonic(&tabs);
	Time::Add(&tabs, ts, &tabs);
restart:
	int ret = sem_timedwait(&m_sem, &tabs);
	if (ret < 0)
	{
		switch(errno)
		{
			case EINTR:
				goto restart;
				break;
			case ETIMEDOUT:
				return -ETIMEDOUT;
				break;
			default:
				abort();
		}
	}
	return 0;
}

