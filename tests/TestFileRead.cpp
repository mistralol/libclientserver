
#include <libclientserver.h>

int main(int argc, char **argv)
{
	std::string buffer;

	if (FileUtil::Read("/proc/cmdline", &buffer) < 0)
		abort();
	//printf("%s", buffer.c_str());

	return 0;
}

