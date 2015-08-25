
#include <libclientserver.h>

int main(int argc, char **argv)
{
	Semaphore sem;
	
	if (alarm(5) != 0)
		abort();
	
	sem.Up();
	sem.Down();
	
	sem.Up();
	if (sem.TryDown() != 0)
		abort();
	if (sem.TryDown() >= 0)
		abort();
		
	struct timespec ts = {0, 100};
	sem.Up();
	if (sem.DownTimeout(&ts) != 0)
		abort();
	if (sem.DownTimeout(&ts) >= 0)
		abort();

	return EXIT_SUCCESS;
}

