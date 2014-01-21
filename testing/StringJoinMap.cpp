
#include <libclientserver.h>

int main(int argc, char **argv)
{
	std::map<std::string, std::string> map;

	map.insert(std::pair<std::string, std::string>("Hello", "World"));
	map.insert(std::pair<std::string, std::string>("Works", "True"));


	std::string str;

	str = String::Join(&map, " ", "=");

	if (str != "Hello=World Works=True")
	{
		fprintf(stderr, "str != \"Hello=World Works=True\"\n");
		fprintf(stderr, "str == %s\n", str.c_str());
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

