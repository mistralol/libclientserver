
#include <libclientserver.h>


int main(int argc, char **argv)
{
	Queue<void *> Q;
	int items = 10;

	for(int i =0;i<items;i++)
	{
		void *p = NULL;
		Q.Add(p);
		printf("Add Item\n");
	}
	
	for(int i =0;i<items;i++)
	{
		void *tmp;
		Q.GetNext(tmp);
		printf("Got Item\n");
	}

	return 0;
};

