
#include <libclientserver.h>

SetUid::SetUid()
{

}

SetUid::~SetUid()
{
	if (setuid(m_uid) < 0)
		abort();
}

int SetUid::Init()
{
	m_uid = getuid();

	if (setuid(0) < 0)
		return -errno;

	if (seteuid(m_uid) < 0)
	{
		printf("seteuid failed: %d\n", errno);
		abort();
	}


	return true;
}

uid_t SetUid::GetUid()
{
	return m_uid;
}


