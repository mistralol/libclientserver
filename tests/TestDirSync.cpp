
#include <libclientserver.h>

int main(int argc, char **argv)
{
	std::string buffer;

	int ret = DirUtil::Sync("/tmp");
	if (ret < 0)
	{
		printf("Failed %s\n", strerror(abs(ret)));
		exit(EXIT_FAILURE);
	}


	return 0;
}

