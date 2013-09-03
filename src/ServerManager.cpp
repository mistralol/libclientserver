
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
	m_Servers.push_back(Server);
	Server->Start(this);
}

void ServerManager::ServerRemove(IServer *Server)
{
	Server->Stop();
	ConnectionRemoveAll(Server);
	abort();
}

void ServerManager::ServerRemoveAll()
{
	while(m_Servers.size() > 0)
	{
		ServerRemove(m_Servers.front());
	}
}

void ServerManager::ConnectionAdd(IServerConnection *Connection)
{
	abort();
}

void ServerManager::ConnectionRemove(IServerConnection *Connection)
{
	abort();
}

void ServerManager::ConnectionRemoveAll()
{
	abort();
}

void ServerManager::ConnectionRemoveAll(IServer *Server)
{
	abort();
}


