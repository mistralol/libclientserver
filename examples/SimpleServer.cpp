
#include <libclientserver.h>

class Handler : public IServerHandler
{
private:
	bool m_Quit;

public:
	Handler()
	{
		m_Quit = false;
	}

	void OnPreNewConnection()
	{
		printf("PreNewConnection\n");
	}

	void OnPostNewConnection(IServerConnection *Connection)
	{
		printf("PostNewConnection\n");
	}
		
	void OnDisconnect(IServerConnection *Connection)
	{
		printf("Disconnect\n");
	}
		
	bool OnRequest(IServerConnection *Connection, Request *request, Request *response)
	{
		std::list<std::string> lst = request->GetArgList();
		std::stringstream ss;

		ss << "OnRequest: " << request->GetCommand() << " ";

		for(std::list<std::string>::iterator it = lst.begin(); it != lst.end(); it++)
		{
			ss << *it << "=" << request->GetArg(*it) << " ";
		}

		printf("%s\n", ss.str().c_str());
//		printf("OnRequest: %s\n", request->GetCommand().c_str());
		return false;
	}
	
	bool OnCommand(IServerConnection *Connection, Request *request)
	{
		printf("OnCommand: %s\n", request->GetCommand().c_str());
		return false;
	}

	void OnBadLine(IServerConnection *Connection, const std::string *line)
	{
		printf("Bad Line: %s\n", line->c_str());
	}

	void Wait()
	{
		while(m_Quit == false)
		{
			printf("Running\n");
			sleep(10);
		}
	}

};

void Start()
{
	Handler SrvHandler;
	ServerManager Manager(&SrvHandler);
	
	ServerUnix Unix("/tmp/SimpleServer");
	
	Manager.ServerAdd(&Unix);
	SrvHandler.Wait();
	Manager.ServerRemove(&Unix);
}

int main(int argc, char **argv)
{
	std::string PFilename = "/tmp/SimpleServer.pid";
	PIDFile PFile(PFilename);
	
	if (PFile.Create() == false)
	{
		printf("Cannot Create PID File '%s'\n", PFilename.c_str());
		return EXIT_FAILURE;
	}
	
	Start();
	
	return EXIT_SUCCESS;
}



