
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
			if (retvalue < 0)
				throw ServerException(ENOSYS, "Unknown Error Or Unsupported command");
			response.SetArg("_ERRNO", "0");
			response.SetArg("_ERROR", "Success");
		}
		catch(ServerException &e)
		{
			Request tmp;

			tmp.SetArg("_ERRNO", Encoder::ToStr(e.GetErrorNo()));
			tmp.SetArg("_ERROR", e.GetErrorMessage());
			response = tmp;
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


