
#include <libclientserver.h>

int main(int argc, char **argv)
{
	std::vector<std::string> vec;

	vec.push_back("Hello");
	vec.push_back("World");
	vec.push_back("Works");

	std::string str;

	str = String::Join(&vec, ",");

	if (str != "Hello,World,Works")
	{
		fprintf(stderr, "str != \"Hello,World,Works\"\n");
		fprintf(stderr, "str == %s\n", str.c_str());
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

