
#include <libclientserver.h>

/**
 * Hex2Str
 * @param[in] str the string value to be decoded
 * @param[out] out the string which was decoded
 * @return True for success and False for failure
 *
 * HexDecode decode a string of hex and returns the same decoded value
 * An example would be "20" will be returned as " "
 */
bool Decoder::ToStr(const std::string &str, std::string &out) {
	const char *c = str.c_str();
	unsigned int x;

	//Must be an even length
	if (str.size() % 2 != 0)
		return false;

	out = "";
	
	while(c[0] != 0) {
		sscanf(c, "%2X", &x);
		out += x;
		c += 2;
	}
	
	return true;
}

bool Decoder::ToLong(const std::string &str, long *value)
{
	std::stringstream ss(str);
	ss >> *value;
	return true;
}

bool Decoder::ToInt(const std::string &str, int *value)
{
	std::stringstream ss(str);
	ss >> *value;
	return true;
}

bool Decoder::ToShort(const std::string &str, short *value)
{
	std::stringstream ss(str);
	ss >> *value;
	return true;
}

bool Decoder::ToChar(const std::string &str, char *value)
{
	std::stringstream ss(str);
	ss >> *value;
	return true;
}

bool Decoder::ToFloat(const std::string &str, float *value)
{
	std::stringstream ss(str);
	ss >> *value;
	return true;
}

bool Decoder::ToDouble(const std::string &str, double *value)
{
	std::stringstream ss(str);
	ss >> *value;
	return true;
}

bool Decoder::ToULong(const std::string &str, unsigned long *value)
{
	std::stringstream ss(str);
	ss >> *value;
	return true;
}

bool Decoder::ToUInt(const std::string &str, unsigned int *value)
{
	std::stringstream ss(str);
	ss >> *value;
	return true;
}

bool Decoder::ToUShort(const std::string &str, unsigned short *value)
{
	std::stringstream ss(str);
	ss >> *value;
	return true;
}

bool Decoder::ToUChar(const std::string &str, unsigned char *value)
{
	std::stringstream ss(str);
	ss >> *value;
	return true;
}

bool Decoder::ToTimeSpec(const std::string &str, struct timespec *ts)
{
	std::stringstream ss(str);
	std::string dummy;
	ss >> ts->tv_sec >> dummy >> ts->tv_nsec;
	return true;
}

bool Decoder::ToTimeVal(const std::string &str, struct timeval *tv)
{
	std::stringstream ss(str);
	std::string dummy;
	ss >> tv->tv_sec >> dummy >> tv->tv_usec;
	return true;
}

