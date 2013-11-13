
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
bool Decoder::Hex2Str(const std::string &str, std::string &out) {
	const char *c = str.c_str();
	unsigned int x;

	out = "";
	
	while(*c != 0) {
		//FIXME: fail on poor data
		sscanf(c, "%2X", &x);
		out += x;
		c += 2;
	}
	
	return true;
}

bool Decoder::Str2Buf(const std::string &str, char **buf, size_t *buflen)
{
	abort();
	return false;
}

bool Decoder::Str2Char(const std::string &str, char value)
{
	abort();
	return false;
}

bool Decoder::Str2UChar(const std::string &str, unsigned char value)
{
	abort();
	return false;
}

bool Decoder::Str2Short(const std::string &str, short value)
{
	abort();
	return false;
}

bool Decoder::Str2UShort(const std::string &str, unsigned short value)
{
	abort();
	return false;
}

bool Decoder::Str2Int(const std::string &str, int value)
{
	abort();
	return false;
}

bool Decoder::Str2UInt(const std::string &str, unsigned int value)
{
	abort();
	return false;
}

bool Str2Long(const std::string &str, long value)
{
	abort();
	return false;
}

bool Str2ULong(const std::string &str, unsigned long value)
{
	abort();
	return false;
}

bool Str2Float(const std::string &str, float value)
{
	abort();
	return false;
}

bool Str2Double(const std::string &str, double value)
{
	abort();
	return false;
}


