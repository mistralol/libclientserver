
#include <libclientserver.h>

int main(int argc, char **argv)
{
	std::string FileName = "/tmp/test.pid";
	PIDFile PFile(FileName);

	if (PFile.Create() == false)
	{
		printf("Failed First PIDFile.Create()\n");
		return EXIT_FAILURE;
	}

	PFile.Remove();

	if (PFile.Create() == false)
	{
		printf("Failed Second PIDFile.Create()\n");
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

