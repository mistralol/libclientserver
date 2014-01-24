
#include <libclientserver.h>


bool TestFails(std::string Filename)
{
	PIDFile PFile(Filename);
	
	if (PFile.Create() == false)
		return true;

	printf("Created PID When Should not have\n");
	return false;
}

int main(int argc, char **argv)
{
	std::string FileName = "/tmp/test.pid";
	PIDFile PFile(FileName);

	if (PFile.Create() == false)
	{
		printf("Failed First PIDFile.Create()\n");
		return EXIT_FAILURE;
	}


	if (TestFails(FileName) == false)
		return EXIT_FAILURE;

	PFile.Remove();

	if (PFile.Create() == false)
	{
		printf("Failed Second PIDFile.Create()\n");
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

