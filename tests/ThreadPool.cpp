
#include <libclientserver.h>

void func(void *arg)
{
	//printf("%s\n", (char *) arg);
}

int main(int argc, char **argv)
{
	ThreadPool Pool;
	static std::string msg = "Hello";

	Pool.Add(func, (void *) msg.c_str());
	Pool.Add(func, (void *) msg.c_str());

	return EXIT_SUCCESS;
}

