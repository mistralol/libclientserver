
#include <libclientserver.h>

void Time::Sleep(const struct timespec *Timeout)
{
	struct timespec rem;
	int ret = nanosleep(Timeout, &rem);
	if (ret < 0)
		abort();
}

