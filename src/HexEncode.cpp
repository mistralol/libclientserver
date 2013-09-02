
#include <stdio.h>
#include <string>

#include <libclientserver.h>

std::string HexEncode(std::string str) {
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

std::string HexDecode(std::string str) {
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

