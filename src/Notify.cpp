
#include <libclientserver.h>

Notify::Notify()
{

}

Notify::~Notify()
{
	if (m_map.size() != 0)
		abort(); //Some things still reference us. This is a bug in the callers code
}

void Notify::Add(INotify *p)
{
	ScopedLock lock(&m_mutex);
	m_map.insert(std::make_pair<INotify *, INotify *>(p, p));
}

void Notify::Remove(INotify *p)
{
	ScopedLock lock(&m_mutex);
	std::map<INotify *, INotify *>::iterator it = m_map.find(p);
	if (it == m_map.end())
		abort(); //Request to delete an item that does not exist
	m_map.erase(it);
}

void Notify::Call(void *d)
{
	ScopedLock lock(&m_mutex);
	for(std::map<INotify *, INotify *>::iterator it = m_map.begin(); it != m_map.end(); it++)
	{
		it->first->Notify(d);
	}
}


