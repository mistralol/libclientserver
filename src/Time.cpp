
#include <libclientserver.h>

boost::system_time Time::CalcTimeout(const struct timespec *Timeout)
{
	boost::system_time timeout = boost::get_system_time();
	timeout += boost::posix_time::seconds(Timeout->tv_sec);
	timeout += boost::posix_time::microseconds(Timeout->tv_nsec / 1000);
	return timeout;
}

boost::system_time Time::CalcTimeout(const struct timeval *Timeout)
{
	boost::system_time timeout = boost::get_system_time();
	timeout += boost::posix_time::seconds(Timeout->tv_sec);
	timeout += boost::posix_time::microseconds(Timeout->tv_usec);
	return timeout;
}

