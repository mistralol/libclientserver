

#include <libclientserver.h>

int main(int argc, char **argv)
{
	if (FileUtil::Exists("/proc/cpuinfo") == false)
		exit(EXIT_FAILURE);
	if (FileUtil::Exists("/proc/notgoingtoexist") == true)
		exit(EXIT_FAILURE);
	return 0;
}

