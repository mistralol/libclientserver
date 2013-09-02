
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
}

void ServerManager::ServerRemove(IServer *Server)
{
	throw;
}

void ServerManager::ServerRemoveAll()
{
	while(m_Servers.size() > 0)
	{
		ServerRemove(m_Servers.front());
	}
}

void ConnectionAdd()
{
	throw;
}

void ConnectionRemove()
{
	throw;
}

void ConnectionRemoveAll()
{
	throw;
}

void ConnectionRemoveAll(IServer *Server)
{
	throw;
}


