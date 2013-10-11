
#include <libclientserver.h>

ServerManager::ServerManager(IServerHandler *Handler)
{
	m_handler = Handler;
}

ServerManager::~ServerManager()
{
	ServerRemoveAll();
	ConnectionRemoveAll();
}
	
void ServerManager::ServerAdd(IServer *Server)
{
	ScopedLock lock(&m_ServersMutex);
	m_Servers.push_back(Server);
	Server->Start(this);
}

void ServerManager::ServerRemove(IServer *Server)
{
	ScopedLock lock(&m_ServersMutex);
	Server->Stop();
	ConnectionRemoveAll(Server);

	std::list<IServer *>::iterator it;
	for(it = m_Servers.begin(); it != m_Servers.end(); it++)
	{
		if (*it == Server)
		{
			m_Servers.erase(it);
			return;
		}
	}
}

void ServerManager::ServerRemoveAll()
{
	ScopedLock lock(&m_ServersMutex);
	while(m_Servers.size() > 0)
	{
		IServer *Server = m_Servers.front();
		Server->Stop();
		ConnectionRemoveAll(Server);
		ServerRemove(m_Servers.front());
	}
}

void ServerManager::ConnectionAdd(IServerConnection *Connection)
{
	ScopedLock lock(&m_ConnectionsMutex);
	m_Connections.push_back(Connection);
}

void ServerManager::ConnectionRemove(IServerConnection *Connection)
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
#ifdef DEBUG
	abort();
#endif
}

void ServerManager::ConnectionRemoveAll()
{
	ScopedLock lock(&m_ConnectionsMutex);
	while(m_Servers.size() > 0)
	{
		ConnectionRemove(m_Connections.front());
	}
}

void ServerManager::ConnectionRemoveAll(IServer *Server)
{
	ScopedLock lock(&m_ConnectionsMutex);
	abort();
}


