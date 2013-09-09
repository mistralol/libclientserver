
#include <libclientserver.h>

#include <boost/algorithm/string.hpp>
#include <boost/asio.hpp>

ClientBase *Client::CreateUnix(const std::string &ConnectionString)
{
	std::vector<std::string> lst;
	boost::split(lst, ConnectionString, boost::is_any_of(":"));

	if (lst.size() != 2)
		return NULL;

	return new ClientUnix(lst[1]);
}

ClientBase *Client::CreateTcp(const std::string &ConnectionString)
{
	abort();
}

ClientBase *Client::Create(const std::string &ConnectionString)
{
	std::vector<std::string> lst;
	boost::split(lst, ConnectionString, boost::is_any_of(":"));

	if (lst.size() == 0)
		return NULL;

	if (lst[0] == "unix")
		return CreateUnix(ConnectionString);
	if (lst[1] == "tcp")
		return CreateTcp(ConnectionString);

	//etc...

	return NULL;
}

