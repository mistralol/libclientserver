
#include <libclientserver.h>

int main(int argc, char **argv)
{
	std::vector<std::string> vec;
	std::string str = "Hello,World";
	
	if (String::Split(&str, ",", &vec) == false)
		return EXIT_FAILURE;

	if (vec[0] != "Hello")
	{
		fprintf(stderr, "vec[0] != Hello\n");
		return EXIT_FAILURE;
	}

	if (vec[1] != "World")
	{
		fprintf(stderr, "vec[0] != World\n");
		return EXIT_FAILURE;
	}

	if (vec.size() != 2)
	{
		fprintf(stderr, "vec.size() != 2\n");
		return EXIT_FAILURE;
	}

	if (String::Split(&str, "!", &vec) != false)
	{
		fprintf(stderr, "Expected function to fail\n");
		return EXIT_FAILURE;
	}

	if (vec.size() != 0)
	{
		fprintf(stderr, "vec.size() != 0\n");
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;	
}

