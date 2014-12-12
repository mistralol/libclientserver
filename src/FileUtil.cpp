
#include <libclientserver.h>

bool FileUtil::Exists(const std::string &fname)
{
	struct stat buf;
	int ret = stat(fname.c_str(), &buf);
	if (ret < 0)
		return false;
	if (S_ISREG(buf.st_mode))
		return true;
	return false;
}

std::string FileUtil::Temp()
{
	std::string fname = "";
	const char *path = getenv("TMP");

	if (!path)
		path = getenv("TEMP");
	if (!path)
		path = "/tmp";

	char *tmp = NULL;
	if (asprintf(&tmp, "%s/fileXXXXXXXXX", path) < 0)
		abort();
	if (tmp == NULL)
		abort();

	char *ptr = mktemp(tmp);
	fname = ptr;
	free(tmp);
	return fname;
}

int FileUtil::Read(const std::string &fname, std::string *buffer)
{
	size_t bufsize = 65535;
	char *buf = (char *) malloc(bufsize);
	FILE *fp = fopen(fname.c_str(), "r");
	if (!fp)
		return -errno;

	if (!buf)
		abort();

	*buffer = "";
	int ret = 0;
	int fd = fileno(fp);
	while(ret >= 0)
	{
		ret = read(fd, buf, bufsize - 1);
		if (ret == 0)
			break;
		if (ret < 0)
		{
			switch(errno)
			{
				case EINTR:
					ret = 0;
					break;
			}
		}
		if (ret > 0)
		{
			buf[ret] = 0;
			*buffer += buf;
		}
	}	

	if (fclose(fp) != 0)
		abort();
	free(buf);
	return 0;
}

int FileUtil::Write(const std::string &fname, const std::string &buffer)
{
	FILE *fp = fopen(fname.c_str(), "w+");
	if (!fp)
		return -errno;
	if (fwrite(buffer.c_str(), buffer.length(), 1, fp) != 1)
		abort();
	if (fclose(fp) != 0)
		abort();
	return 0;
}

int FileUtil::WriteSync(const std::string &fname, const std::string &buffer)
{
	FILE *fp = fopen(fname.c_str(), "w+");
	if (!fp)
		return -errno;
	if (fwrite(buffer.c_str(), buffer.length(), 1, fp) != 1)
		abort();

	if (fflush(fp) != 0)
		abort();

	if (fsync(fileno(fp)) != 0)
		abort();

	if (fclose(fp) != 0)
		abort();
	return 0;
}

int FileUtil::Sync(const std::string &fname)
{
	FILE *fp = fopen(fname.c_str(), "r");
	if (!fp)
		return -errno;
	if (fsync(fileno(fp)) != 0)
	{
		int err = errno;
		if (fclose(fp) != 0)
			abort();
		return -err;
	}
	if (fclose(fp) != 0)
		abort();
	return 0;
}


