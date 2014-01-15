
#include <libclientserver.h>

int main(int argc, char **argv)
{
	std::list<std::string> lst;
	std::string str = "Hello,World";
	
	if (String::Split(&str, ",", &lst) == false)
		return EXIT_FAILURE;

	if (lst.front() != "Hello")
	{
		fprintf(stderr, "lst[0] != Hello\n");
		return EXIT_FAILURE;
	}

	if (lst.back() != "World")
	{
		fprintf(stderr, "lst[0] != World\n");
		return EXIT_FAILURE;
	}

	if (lst.size() != 2)
	{
		fprintf(stderr, "lst.size() != 2\n");
		return EXIT_FAILURE;
	}

	if (String::Split(&str, "!", &lst) != false)
	{
		fprintf(stderr, "Expected function to fail\n");
		return EXIT_FAILURE;
	}

	if (lst.size() != 0)
	{
		fprintf(stderr, "vec.size() != 0\n");
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;	
}

