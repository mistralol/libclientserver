
#include <libclientserver.h>


GroupID::GroupID()
{

}

GroupID::~GroupID()
{
	while(m_stack.empty() == false)
	{
		if (Down() < 0)
			abort();
	}
}

int GroupID::Up(uid_t id)
{
	uid_t current = getegid();
	if (setegid(id) < 0)
		return -errno;
	m_stack.push(current);
	return 0;
}

int GroupID::Down()
{
	uid_t id = m_stack.top();
	if (setegid(id) < 0)
		return -errno;
	m_stack.pop();
	return 0;
}


