
#include <libclientserver.h>

static void func(const char *str)
{
	//printf("%s\n", (char *) arg);
}

int main(int argc, char **argv)
{
	ThreadPool Pool;
	static std::string msg = "Hello";

	//std::function<void()> f1 = std::bind(func, "Hello");
	std::function<void()> f1 = std::bind(func, msg.c_str());
	Pool.Add(f1);
	std::function<void()> f2 = std::bind(func, "Hello");
	Pool.Add(f2);

	return EXIT_SUCCESS;
}

