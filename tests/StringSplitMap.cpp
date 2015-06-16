
#include <libclientserver.h>

int main(int argc, char **argv)
{
	std::list<std::string> lst;
	std::string str = "HELLO=World Work=True";
	std::map<std::string, std::string> map;
	std::map<std::string, std::string>::iterator it;

	if (String::Split(&str, " ", "=", &map) == false)
	{
		fprintf(stderr, "Failed Basic Decode\n");
		return EXIT_FAILURE;
	}

	it = map.find("HELLO");
	if (it == map.end())
	{
		fprintf(stderr, "Failed to find 'HELLO' in map\n");
		return EXIT_FAILURE;
	}

	if (it->second != "World")
	{
		fprintf(stderr, "it->second != World\n");
		fprintf(stderr, "it->second == %s\n", it->second.c_str());
		return EXIT_FAILURE;
	}

	it = map.find("Work");
	if (it == map.end())
	{
		fprintf(stderr, "Failed to find 'Work' in map\n");
		return EXIT_FAILURE;
	}

	if (it->second != "True")
	{
		fprintf(stderr, "it->second != True\n");
		fprintf(stderr, "it->second == %s\n", it->second.c_str());
		return EXIT_FAILURE;
	}

	if (map.size() != 2)
	{
		fprintf(stderr, "map.size != 2\n");
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;	
}

