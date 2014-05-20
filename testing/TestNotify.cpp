
#include <libclientserver.h>

bool Called = false;

class TestNotify : public INotify
{
	void Notify(void *p)
	{
		//std::string *x = (std::string *) p;
		Called = true;
	}
};

int main(int argc, char **argv)
{
	std::string Test = "Hello Word";
	TestNotify n1;
	TestNotify n2;
	Notify lst;

	lst.Add(&n1);
	lst.Add(&n2);

	lst.Call(&Test);
	lst.Call(&Test);

	lst.Remove(&n2);

	lst.Call(&Test);

	lst.Remove(&n1);

	if (Called)
		return EXIT_SUCCESS;
	return EXIT_FAILURE;
}

