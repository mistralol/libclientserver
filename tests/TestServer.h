
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
		//printf("PreNewConnection\n");
	}

	void OnPostNewConnection(IServerConnection *Connection)
	{
		//printf("PostNewConnection\n");
	}
		
	void OnDisconnect(IServerConnection *Connection)
	{
		//printf("Disconnect\n");
	}
		
	int OnRequest(IServerConnection *Connection, Request *request, Request *response)
	{
		std::list<std::string> lst = request->GetArgList();
		std::stringstream ss;
		std::string Command = request->GetCommand();

		ss << "OnRequest: " << request->GetCommand() << " ";

		for(std::list<std::string>::iterator it = lst.begin(); it != lst.end(); it++)
		{
			ss << *it << "=" << request->GetArg(*it) << " ";
		}

		printf("%s\n", ss.str().c_str());

		if (Command == "PING")
			return 0;

		if (Command == "QUIT")
		{
			m_Quit = true;
			return 0;
		}

		
		return -ENOSYS;
	}
	
	int OnCommand(IServerConnection *Connection, Request *request)
	{
		printf("OnCommand: %s\n", request->GetCommand().c_str());
		return -ENOSYS;
	}
	
	int OnJsonRequest(IServerConnection *Connection, Json::Value *req, Json::Value *res)
	{
		printf("OnJsonRequest\n");
		return -ENOSYS;
	}
	
	int OnJsonCommand(IServerConnection *Connection, Json::Value *req)
	{
		printf("OnJsonCommand\n");
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

class TestServer : Thread
{
	public:
		TestServer()
		{
			m_quit = false;
			Start();
		}

		~TestServer()
		{
			m_quit = true;
			Stop();
		}

		virtual void Run()
		{
			Handler SrvHandler;
			ServerManager Manager(&SrvHandler);
	
			ServerUnix Unix("/tmp/TestServer");
	
			Manager.ServerAdd(&Unix);
			while(m_quit == false)
			{
				Request event;
				event.SetCommand("RUNNING");
				event.SetID(1);
				Manager.SendEvent(&event);

				sleep(1);
			}

			Manager.ServerRemove(&Unix);
		}
	private:
		bool m_quit;
};




