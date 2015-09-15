

#include <libclientserver.h>

int main(int argc, char **argv)
{
	std::string pass = "password";
	std::string salt = "salting";
	
	std::string hash1 = Crypto::SHA1Pass(pass, salt);
	printf("%s\n", hash1.c_str());
	std::string hash2 = Crypto::SHA1Pass(pass, salt);
	
	if (hash1 != hash2)
	{
		printf("Hash1 != Hash2\n");
		exit(EXIT_FAILURE);
	}
	
	std::string hash3 = Crypto::SHA1Pass(pass, "different salt");
	
	if (hash1 == hash3)
	{
		printf("Hash1 != Hash2\n");
		exit(EXIT_FAILURE);
	}
	
	return EXIT_SUCCESS;
}

