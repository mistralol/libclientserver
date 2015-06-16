
#include <libclientserver.h>

int main(int argc, char ** argv)
{
	Process Proc("/bin/sleep");
	Proc.ArgsAdd("5");

	if (Proc.Start() < 0)
	{
		printf("Failed To Start Process\n");
		abort();
	}

	if (Proc.WaitforExit() == false)
	{
		printf("Failed To Wait for Exit\n");
		abort();
	}

	return 0;
}
