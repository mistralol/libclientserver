
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


