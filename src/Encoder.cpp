
#include <libclientserver.h>

/**
 * Str2Hex
 * @param[in] str the string value to be encoded
 * @return The encoded value
 *
 * HexEncode encodes a string and returns the same string encoded in hex.
 * An example would be " " will be returned as "20"
 */
std::string Encoder::Str2Hex(const std::string &str) {
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

std::string Encoder::Buf2Hex(const char *c, size_t buflen)
{
	std::string tmp;
	char buf[3];
	
	for(size_t i = 0;i<buflen;i++)
	{
		sprintf(buf, "%02X", (unsigned char) c[i]);
		tmp += buf;
	}

	return tmp;
}

std::string Encoder::Char2Str(char value)
{
	char buf[2];
	sprintf(buf, "%c", value);
	return std::string(buf);
}

std::string Encoder::UChar2Str(unsigned char value)
{
	char buf[32];
	sprintf(buf, "%d", value);
	return std::string(buf);
}

std::string Encoder::Short2Str(short value)
{
	char buf[32];
	sprintf(buf, "%d", value);
	return std::string(buf);
}

std::string Encoder::UShort2Str(unsigned short value)
{
	char buf[32];
	sprintf(buf, "%d", value);
	return std::string(buf);
}

std::string Encoder::Int2Str(int value)
{
	char buf[32];
	sprintf(buf, "%d", value);
	return std::string(buf);
}

std::string Encoder::UInt2Str(unsigned int value)
{
	char buf[32];
	sprintf(buf, "%d", value);
	return std::string(buf);
}

std::string Encoder::Long2Str(long value)
{
	char buf[32];
	sprintf(buf, "%ld", value);
	return std::string(buf);
}

std::string Encoder::ULong2Str(unsigned long value)
{
	char buf[32];
	sprintf(buf, "%lu", value);
	return std::string(buf);
}

std::string Encoder::Float2Str(float value)
{
	char buf[32];
	sprintf(buf, "%f", value);
	return std::string(buf);
}

std::string Encoder::Double2Str(double value)
{
	char buf[32];
	sprintf(buf, "%f", value);
	return std::string(buf);
}


