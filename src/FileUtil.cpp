
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

int FileUtil::Read(const std::string &fname, std::string *buffer)
{
	size_t bufsize = 65535;
	char *buf = (char *) malloc(bufsize);
	if (!buf)
		return -errno;

	FILE *fp = fopen(fname.c_str(), "r");
	if (!fp)
	{
		free(buf);
		return -errno;
	}

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
				default:
					if (fclose(fp) != 0)
						abort();
					free(buf);
					return -errno;
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

int FileUtil::PathList(std::vector<std::string> *lst)
{
	char *path = getenv("PATH");
	if (!path)
		return -1;
	lst->clear();
	std::string str = path;
	if (String::Split(&str, ":", lst) == false)
	{
		lst->clear();
		return -1;
	}
	return lst->size();
}

int FileUtil::PathSearch(const std::string &name, std::string *location)
{
	std::vector<std::string> lst;
	
	if (PathList(&lst) < 0)
		return -1;

	for(std::vector<std::string>::iterator it = lst.begin(); it != lst.end(); it++)
	{
		struct stat info;
		std::string tname = *it + '/' + name;
		
		if (stat(tname.c_str(), &info) == 0)
		{
			if (S_ISREG(info.st_mode) || S_ISLNK(info.st_mode))
			{
				*location = tname;
				return 1;
			}
		}
	}	
	return -1;
}


