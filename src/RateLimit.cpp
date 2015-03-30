
#include <libclientserver.h>

RateLimit::RateLimit()
{
	m_last = {0, 0};
}

RateLimit::~RateLimit()
{

}

void RateLimit::SetTimeout(const struct timespec *tv)
{
	memcpy(&m_timeout, tv, sizeof(*tv));
}

bool RateLimit::Check()
{
	struct timespec now;
	struct timespec when;
	Time::MonoTonic(&now);
	Time::Add(&now, &m_timeout, &when);

	if (Time::IsGreater(&now, &when))
	{
		memcpy(&m_last, &now, sizeof(now));
		return true;
	}
	return false;

}

