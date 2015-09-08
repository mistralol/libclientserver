
#include <libclientserver.h>


int main(int argc, char **argv)
{
	Request req;
	std::vector<std::string> lst, ans;
	
	lst.push_back("hello");
	lst.push_back("world");
	
	req.SetArg("lst", &lst);
	
	if (req.GetVectorString("lst", &ans) == false)
	{
		printf("Failed to decode 1\n");
		exit(EXIT_FAILURE);
	}
	
	if (ans.size() != lst.size())
	{
		printf("Failed sizes dont match\n");
		exit(EXIT_FAILURE);
	}

	for(size_t i = 0; i < lst.size(); i++)
	{
		if (lst[i] != ans[i])
		{
			printf("Answers Dont match %s != %s\n", lst[i].c_str(), ans[i].c_str());
			exit(EXIT_FAILURE);
		}
	}
	
	lst.clear();
	ans.clear();
	lst.push_back("hello");
	
	req.SetArg("lst", &lst);
	if (req.GetVectorString("lst", &ans) == false)
	{
		printf("Failed to decode 2\n");
		exit(EXIT_FAILURE);
	}
	
	if (ans.size() != lst.size())
	{
		printf("Failed sizes dont match\n");
		exit(EXIT_FAILURE);
	}

	for(size_t i = 0; i < lst.size(); i++)
	{
		if (lst[i] != ans[i])
		{
			printf("Answers Dont match %s != %s\n", lst[i].c_str(), ans[i].c_str());
			exit(EXIT_FAILURE);
		}
	}

	return 0;
}

