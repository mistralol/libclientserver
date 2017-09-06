
#include <libclientserver.h>

ClientBase *Client::CreateUnix(const std::string &ConnectionString)
{
	std::vector<std::string> lst;
	String::Split(&ConnectionString, ":", &lst);

	if (lst.size() != 2)
		return NULL;

	return new ClientUnix(lst[1]);
}

ClientBase *Client::CreateUnixSelected(const std::string &ConnectionString)
{
	std::vector<std::string> lst;
	String::Split(&ConnectionString, ":", &lst);

	if (lst.size() != 2)
		return NULL;

	return new ClientUnixSelected(lst[1]);
}

ClientBase *Client::CreateTcp(const std::string &ConnectionString)
{
	std::vector<std::string> lst;
	String::Split(&ConnectionString, ":", &lst);

	if (lst.size() != 3)
		return NULL;

	return new ClientTCP(atoi(lst[2].c_str()), lst[1]);
}

ClientBase *Client::Create(const std::string &ConnectionString)
{
	std::vector<std::string> lst;
	String::Split(&ConnectionString, ":", &lst);

	if (lst.size() == 0)
		return NULL;

	if (lst[0] == "unix")
		return CreateUnix(ConnectionString);
	if (lst[0] == "unixselected")
		return CreateUnix(ConnectionString);
	if (lst[0] == "tcp")
		return CreateTcp(ConnectionString);

	//etc...

	return NULL;
}

