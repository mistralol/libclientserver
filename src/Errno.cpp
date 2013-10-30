
#include <libclientserver.h>

/**
 * ToStr
 * @retrun Return the string error of errno
 *
 * Convert errno to a string
 */
const std::string Errno::ToStr()
{
	return ToStr(errno);
}

/**
 * ToStr
 * @param[in] err
 * @return A string meaning of the error number
 *
 * Convert err to a string meaning of the error
 */
const std::string Errno::ToStr(int err)
{
	char buf[512];
	std::string str;

	str = strerror_r(err, buf, sizeof(buf));
	return str;
}

