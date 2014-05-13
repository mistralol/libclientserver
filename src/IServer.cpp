
#include <libclientserver.h>


void IServer::ConnectionAdd(IServerConnection *Connection)
{
	ScopedLock lock(&m_ConnectionsMutex);
	m_Connections.push_back(Connection);
	
}

void IServer::ConnectionRemove(IServerConnection *Connection)
{
	ScopedLock lock(&m_ConnectionsMutex);
	std::list<IServerConnection *>::iterator it;
	for(it = m_Connections.begin(); it != m_Connections.end(); it++)
	{
		if (*it == Connection)
		{
			m_Connections.erase(it);
			return;
		}
	}
}

void IServer::ConnectionRemoveAll()
{
	ScopedLock lock(&m_ConnectionsMutex);
	while(m_Connections.size() > 0)
	{
		ConnectionRemove(m_Connections.front());
	}
}

void IServer::SendEvent(Request *event)
{
	ScopedLock lock(&m_ConnectionsMutex);
	std::list<IServerConnection *>::iterator it;
	for(it = m_Connections.begin(); it != m_Connections.end(); it++)
	{
		(*it)->SendEvent(event);
	}
}

