
#include <libclientserver.h>

int main(int argc, char **argv)
{
	std::list<std::string> lst;

	lst.push_back("Hello");
	lst.push_back("World");
	lst.push_back("Works");

	std::string str;

	str = String::Join(&lst, ",");

	if (str != "Hello,World,Works")
	{
		fprintf(stderr, "str != \"Hello,World,Works\"\n");
		fprintf(stderr, "str == %s\n", str.c_str());
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

