
#include <stdio.h>
#include <string>

#include <libclientserver.h>

/**
 * HexEncode
 * @param[in] str the string value to be encoded
 * @return The encoded value
 *
 * HexEncode encodes a string and returns the same string encoded in hex.
 * An example would be " " will be returned as "20"
 */
std::string HexEncode(const std::string &str) {
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

/**
 * HexEncode
 * @param[in] str the string value to be decoded
 * @return The decoded value
 *
 * HexDecode decode a string of hex and returns the same decoded value
 * An example would be "20" will be returned as " "
 */
std::string HexDecode(const std::string &str) {
	std::string tmp;
	const char *c = str.c_str();
	unsigned int x;
	
	while(*c != 0) {
		sscanf(c, "%2X", &x);
		tmp += x;
		c += 2;
	}
	
	return tmp;
}

