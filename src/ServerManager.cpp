
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
	RaisePostNewConnection(Connection);
}

void ServerManager::ConnectionRemove(IServerConnection *Connection)
{
	RaiseDisconnect(Connection);
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

bool ServerManager::ProcessLine(IServerConnection *Connection, const std::string *line)
{
	std::string command = "";
	std::string args = "";

	if (String::SplitOne(line, &command, &args, " ") == false)
	{
		RaiseBadLine(Connection, line);
		return false;
	}

	if (command == "REQUEST")
	{
		Request request;
		Request response;

		if (request.Decode(&args) == false)
		{
			RaiseBadLine(Connection, line);
			return false;
		}
		
		m_TotalRequests++;
		bool retvalue = RaiseRequest(Connection, &request, &response);
		if (retvalue == false)
		{
			//FIXME: Send default error response
			//FIXME: Send Error By Exception?
			return false;
		}

		std::string ResponseStr = response.Encode();
		return Connection->SendLine(&ResponseStr);
	}

	if (command == "COMMAND")
	{
		Request request;

		if (request.Decode(&args) == false)
		{
			RaiseBadLine(Connection, line);
			return false;
		}

		m_TotalCommands++;
		return RaiseCommand(Connection, &request);
	}

	RaiseBadLine(Connection, line);
	return false;
}

void ServerManager::RaisePreNewConnection()
{
	m_handler->OnPreNewConnection();
}

void ServerManager::RaisePostNewConnection(IServerConnection *Connection)
{
	m_handler->OnPostNewConnection(Connection);
}

void ServerManager::RaiseDisconnect(IServerConnection *Connection)
{
	m_handler->OnDisconnect(Connection);
}

void ServerManager::RaiseBadLine(IServerConnection *Connection, const std::string *line)
{
	m_handler->OnBadLine(Connection, line);
}

bool ServerManager::RaiseRequest(IServerConnection *Connection, Request *request, Request *response)
{
	return m_handler->OnRequest(Connection, request, response);
}

bool ServerManager::RaiseCommand(IServerConnection *Connection, Request *request)
{
	return m_handler->OnCommand(Connection, request);
}


