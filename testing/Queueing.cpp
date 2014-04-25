
#include <libclientserver.h>


int main(int argc, char **argv)
{
	Queue<void *> Q;
	int items = 10;

	for(int i =0;i<items;i++)
	{
		Q.Add(NULL);
		printf("Add Item\n");
	}
	
	for(int i =0;i<items;i++)
	{
		Q.GetNext();
		printf("Got Item\n");
	}


	return 0;
};

