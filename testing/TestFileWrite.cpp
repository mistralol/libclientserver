
#include <libclientserver.h>

int main(int argc, char **argv)
{

	std::string data = "Hello World";
	std::string buf;
	std::string fname = FileUtil::Temp();

	if (FileUtil::Write(fname, data) < 0)
		abort();
	if (FileUtil::Read(fname, &buf) < 0)
		abort();
	if (data != buf)
		abort();
	if (unlink(fname.c_str()) < 0)
		abort();
	return 0;
}

