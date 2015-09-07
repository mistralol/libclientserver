
#include <stdio.h>

#include <libclientserver.h>

int main(int argc, char **argv)
{
	Uuid uuid = Uuid();

	printf("%s\n", uuid.ToString().c_str());


	std::string tmp = uuid.ToString();

	Uuid *id = Uuid::TryParse(tmp);
	if (id == NULL)
	{
		printf("Failed on first TryParse\n");
		return EXIT_FAILURE;
	}

	delete id;

	id = Uuid::TryParse("hjhhjhjk");
	if (id != NULL)
	{
		printf("Failed on second TryParse\n");
		return EXIT_FAILURE;
	}
	delete id;

	return EXIT_FAILURE;
}

