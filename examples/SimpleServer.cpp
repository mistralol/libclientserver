
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
		
	int OnRequest(IServerConnection *Connection, Json::Value &req, Json::Value &res)
	{
		if (req.isMember("action") && req["action"].isString())
		{
			std::string action = req["action"].asString();
			//printf("OnJsonRequest %s\n", action.c_str());
			
			if (action == "PING")
			{
				res["data"] = "PONG";
				return 0;
			}
			else if (action == "SLEEP")
			{
				if (req.isMember("delay") && req["delay"].isInt())
				{
					int delay = req["delay"].asInt();
					sleep(delay);
					return 0;
				}
			}
			else if (action == "THROW")
			{
				throw(ServerException(0, "Some Exception"));
			}
			else
			{
				printf("OnJsonRequest Unknown Action '%s'", action.c_str());
				return -EINVAL;
			}

		}
		else
		{
			printf("OnJsonRequest No Action\n");
		}
		return -ENOSYS;
	}
	
	int OnCommand(IServerConnection *Connection, Json::Value &req)
	{
		if (req.isMember("action") && req["action"].isString())
		{
			std::string action = req["action"].asString();
			printf("OnJsonCommand %s\n", action.c_str());
			return 0;
		}
		else
		{
			printf("OnJsonCommand No Action\n");
			return 0;
		}
		
		return -ENOSYS;
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
			sleep(1);
		}
	}

};

static void Start()
{
	Handler SrvHandler;
	ServerManager Manager(&SrvHandler);
	Manager.SetThreads(4);
//	ServerUnix Unix("/tmp/SimpleServer");
//	ServerUnixSelected Unix("/tmp/SimpleServer");
	ServerUnixPolled Unix("/tmp/SimpleServer");
	
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
	
	printf("Main Thread Exit\n");
	return EXIT_SUCCESS;
}



