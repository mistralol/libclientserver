
#include <libclientserver.h>

int main(int argc, char **argv)
{
	Optional<std::string *> opt1;
	Optional<std::string> opt2;
	
	if (opt1)
	{
		printf("Should not be set\n");
		exit(EXIT_FAILURE);
	}
	
	opt1 = new std::string("Hello");
	if (!opt1)
	{
		printf("Should be set\n");
		exit(EXIT_FAILURE);
	}
	
	
	delete opt1.get();
	opt1.Reset();
	
	opt1.Reset(new std::string("Boo"));
	
	printf("Working: %s\n", opt1.get()->c_str());
	
	delete opt1.get();
	
	return 0;
}

