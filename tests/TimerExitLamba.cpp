
#include <libclientserver.h>

int main(int argc, char **argv)
{
	Timers Tmr;

	alarm(30);

	TimerFunc Lamba(5, []() { _exit(0); });
	Tmr.Add(&Lamba);
	while(1) {
		sleep(1);
	}
	Tmr.Remove(&Lamba);

	return 0;	
}
