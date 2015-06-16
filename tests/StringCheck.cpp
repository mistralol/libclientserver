#include <libclientserver.h>

int main(int argc, char **argv)
{
	std::string str1 = "ABC";

	if (String::Check(&str1, "DEF") == true)
	{
		printf("Failed First Test\n");
		return EXIT_FAILURE;
	}

	if (String::Check(&str1, "DEFGDSDSGDSGSDA") == false)
	{
		printf("Failed second Test\n");
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

