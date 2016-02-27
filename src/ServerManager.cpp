
#include <libclientserver.h>

ServerManager::ServerManager(IServerHandler *Handler)
{
	m_handler = Handler;
}

ServerManager::~ServerManager()
{
	ServerRemoveAll();
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
		ServerRemove(m_Servers.front());
	}
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
		try
		{
			int retvalue = RaiseRequest(Connection, &request, &response);
			response.SetArg("_ERRNO", retvalue);
			response.SetArg("_ERROR", strerror(retvalue));
		}
		catch(ServerException &e)
		{
			response.SetArg("_ERRNO", Encoder::ToStr(e.GetErrorNo()));
			response.SetArg("_ERROR", e.GetErrorMessage());
			std::string msg = e.what();
			response.SetArg("_EXCEPTION", msg);
		}
		catch(std::exception &e)
		{
			std::string msg = e.what();
			response.SetArg("_EXCEPTION", msg);
		}

		response.SetCommand(request.GetCommand());
		response.SetID(request.GetID());

		std::string ResponseStr = "RESPONSE " + response.Encode() + "\n";
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

int ServerManager::RaiseRequest(IServerConnection *Connection, Request *request, Request *response)
{
	return m_handler->OnRequest(Connection, request, response);
}

int ServerManager::RaiseCommand(IServerConnection *Connection, Request *request)
{
	return m_handler->OnCommand(Connection, request);
}

void ServerManager::SendEvent(Request *event)
{
	ScopedLock lock(&m_ServersMutex);

	std::list<IServer *>::iterator it;
	for(it = m_Servers.begin(); it != m_Servers.end(); it++)
	{
		(*it)->SendEvent(event);
	}
}


