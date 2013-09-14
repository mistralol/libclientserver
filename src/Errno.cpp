
#include <libclientserver.h>

const std::string Errno::ToStr()
{
	return ToStr(errno);
}

const std::string Errno::ToStr(int err)
{
	char buf[512];
	std::string str;

	str = strerror_r(err, buf, sizeof(buf));
	return str;
}

