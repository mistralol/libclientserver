
#include <libclientserver.h>


UserID::UserID()
{

}

UserID::~UserID()
{
	while(m_stack.empty() == false)
	{
		if (Down() < 0)
			abort();
	}
}

int UserID::Up(uid_t id)
{
	uid_t current = geteuid();
	if (seteuid(id) < 0)
		return -errno;
	m_stack.push(current);
	return 0;
}

int UserID::Down()
{
	uid_t id = m_stack.top();
	if (seteuid(id) < 0)
		return -errno;
	m_stack.pop();
	return 0;
}

uid_t GetUIDFromName(const std::string username)
{
	int bufsize = sysconf(_SC_GETPW_R_SIZE_MAX);
	if (bufsize < 0)
		bufsize = 65535;

	char *buf = (char *) malloc(bufsize * sizeof(char));
	if (!buf)
		return -ENOMEM;

	struct passwd pwd, *pwd2;
	if (getpwnam_r(username.c_str(), &pwd, buf, bufsize, &pwd2) < 0)
	{
		free(buf);
		return -errno;
	}

	uid_t value = pwd.pw_uid;
	free(buf);
	return value;
}

gid_t GetGIDFromName(const std::string username)
{
	int bufsize = sysconf(_SC_GETPW_R_SIZE_MAX);
	if (bufsize < 0)
		bufsize = 65535;

	char *buf = (char *) malloc(bufsize * sizeof(char));
	if (!buf)
		return -ENOMEM;

	struct passwd pwd, *pwd2;
	if (getpwnam_r(username.c_str(), &pwd, buf, bufsize, &pwd2) < 0)
	{
		free(buf);
		return -errno;
	}

	uid_t value = pwd.pw_gid;
	free(buf);
	return value;
}

