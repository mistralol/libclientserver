
#include <libclientserver.h>

/**
 * Barrier
 *
 * Constructor for Barrier. This will never normally fail except for OOM
 */
Barrier::Barrier()
{
	m_fired = false;
}

Barrier::~Barrier()
{

}

/**
 * Wait
 *
 * This function should be called to block the current thread.
 */
void Barrier::Wait()
{
	ScopedLock lock(&m_mutex);
	while(m_fired == false)
	{
		m_mutex.Wait();
	}
}

/**
 * WakeUp
 *
 * This function should be called to release the barrier and permit any
 * current and future calls to Wait not to block
 */
void Barrier::WakeUp()
{
	ScopedLock lock(&m_mutex);
	m_fired = true;
	m_mutex.WakeUpAll();
}

/**
 * Reset
 *
 * This function will reset the barrier to the original state.
 * You should take care that no functions should be sleeping on the barrier at the time this function is called
 */
void Barrier::Reset()
{
	ScopedLock lock(&m_mutex);
	m_fired = false;
}

