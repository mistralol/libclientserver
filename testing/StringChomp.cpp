
#include <libclientserver.h>

int main(int argc, char **argv)
{
	std::string str1 = "Hello\n";
	std::string str2 = "Hello\r\n";

	if (String::Chomp(&str1, '\n') != "Hello")
	{
		printf("Failed First Test\n");
		return EXIT_FAILURE;
	}

	if (String::Chomp(&str1, "\n") != "Hello")
	{
		printf("Failed Second Test\n");
		return EXIT_FAILURE;
	}

	if (String::Chomp(&str2, "\n\r") != "Hello")
	{
		printf("Failed Third Test\n");
		return EXIT_FAILURE;
	}

	if (String::Chomp(&str2, "\r\n") != "Hello")
	{
		printf("Failed Fourth Test\n");
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

