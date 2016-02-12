
#include <iostream>
#include <libclientserver.h>


int main(int argc, char **argv)
{
	struct timespec ts = {10, 0};

	uint64_t value = Time::NanoSeconds(&ts);

	std::cout << value << std::endl;
	if (value != 10000000000)
		abort();

	return 0;
}

