
#include <libclientserver.h>

/**
 * ToStr
 * @param[in] str the string value to be encoded
 * @return The encoded value
 *
 * Str2Hex encodes a string and returns the same string encoded in hex.
 * An example would be " " will be returned as "20"
 */
std::string Encoder::ToStr(const std::string &str) {
	std::string tmp;
	const char *c = str.c_str();
	char buf[3];
	
	while(*c != 0) {
		sprintf(buf, "%02X", (unsigned char) *c);
		tmp += buf;
		c++;
	}

	return tmp;
}

std::string Encoder::ToStr(const long value)
{
	std::stringstream ss;
	ss << value;
	return ss.str();
}

std::string Encoder::ToStr(const int value)
{
	std::stringstream ss;
	ss << value;
	return ss.str();
}

std::string Encoder::ToStr(const short value)
{
	std::stringstream ss;
	ss << value;
	return ss.str();
}

std::string Encoder::ToStr(const char value)
{
	std::stringstream ss;
	ss << value;
	return ss.str();
}

std::string Encoder::ToStr(const float value)
{
	std::stringstream ss;
	ss << value;
	return ss.str();
}

std::string Encoder::ToStr(const double value)
{
	std::stringstream ss;
	ss << value;
	return ss.str();
}

std::string Encoder::ToStr(const unsigned long value)
{
	std::stringstream ss;
	ss << value;
	return ss.str();
}

std::string Encoder::ToStr(const unsigned int value)
{
	std::stringstream ss;
	ss << value;
	return ss.str();
}

std::string Encoder::ToStr(const unsigned short value)
{
	std::stringstream ss;
	ss << value;
	return ss.str();
}

std::string Encoder::ToStr(const unsigned char value)
{
	std::stringstream ss;
	ss << value;
	return ss.str();
}

std::string Encoder::ToStr(const struct timespec *ts)
{
	std::stringstream ss;
	ss << ts->tv_sec << ':' << ts->tv_nsec;
	return ss.str();
}

std::string Encoder::ToStr(const struct timeval *tv)
{
	std::stringstream ss;
	ss << tv->tv_sec << ':' << tv->tv_usec;
	return ss.str();
}


