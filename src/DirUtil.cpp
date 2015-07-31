
#include <libclientserver.h>

int DirUtil::Sync(const std::string &dirname)
{
	int ret = 0;
	DIR *d = opendir(dirname.c_str());
	if (!d)
		return -errno;
	
	int fd = dirfd(d);
	if (fd < 0)
		abort();
	
	ret = fsync(fd);
	
	if (closedir(d) < 0)
		abort();

	return ret;
}

