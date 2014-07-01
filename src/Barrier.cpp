
#include <libclientserver.h>

Barrier::Barrier()
{
	m_fired = false;
}

Barrier::~Barrier()
{

}

void Barrier::Wait()
{
	ScopedLock lock(&m_mutex);
	while(m_fired == false)
	{
		m_mutex.Wait();
	}
}

void Barrier::WakeUp()
{
	ScopedLock lock(&m_mutex);
	m_fired = true;
	m_mutex.WakeUpAll();
}

