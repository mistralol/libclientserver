
#include <libclientserver.h>

int main(int argc, char **argv)
{
	std::string str = "Hello,World,Extra";
	std::string left;
	std::string right;
	
	if (String::SplitOne(&str, &left, &right, ",") == false)
	{
		fprintf(stderr, "Function failed\n");
		return EXIT_FAILURE;
	}

	if (left != "Hello")
	{
		fprintf(stderr, "left != Hello\n");
		return EXIT_FAILURE;
	}

	if (right != "World,Extra")
	{
		fprintf(stderr, "right != World,Extra\n");
		return EXIT_FAILURE;
	}

	if (String::SplitOne(&str, &left, &right, "!") != false)
	{
		fprintf(stderr, "Expected function to fail\n");
		return EXIT_FAILURE;

		if (left != "")
		{
			fprintf(stderr, "left != \"\"\n");
			return EXIT_FAILURE;
		}

		if (right != "")
		{
			fprintf(stderr, "left != \"\"\n");
			return EXIT_FAILURE;
		}
	}

	return EXIT_SUCCESS;	
}

