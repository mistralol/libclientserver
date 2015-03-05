
#include <libclientserver.h>

SetGid::SetGid()
{

}

SetGid::~SetGid()
{
	if (setgid(m_gid) < 0)
		abort();
}

int SetGid::Init()
{
	m_gid = getgid();

	if (setgid(0) < 0)
		return -errno;

	if (setegid(m_gid) < 0)
	{
		printf("setegid failed: %d\n", errno);
		abort();
	}


	return true;
}

uid_t SetGid::GetGid()
{
	return m_gid;
}


