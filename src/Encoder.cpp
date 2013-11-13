
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


std::string Encoder::Buf2Hex(const char *buf, size_t buflen)
{
	abort();
}

std::string Encoder::Char2Str(char value)
{
	abort();
}

std::string Encoder::UChar2Str(unsigned char value)
{
	abort();
}

std::string Encoder::Short2Str(short value)
{
	abort();
}

std::string Encoder::UShort2Str(unsigned short value)
{
	abort();
}

std::string Encoder::Int2Str(int value)
{
	abort();
}

std::string Encoder::UInt2Str(unsigned int value)
{
	abort();
}

std::string Encoder::Long2Str(long value)
{
	abort();
}

std::string Encoder::ULong2Str(unsigned long value)
{
	abort();
}

std::string Encoder::Float2Str(float value)
{
	abort();
}

std::string Encoder::Double2Str(double value)
{
	abort();
}


