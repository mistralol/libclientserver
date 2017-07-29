
#include <libclientserver.h>

int retval = 0;

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
		
	int OnRequest(IServerConnection *Connection, Json::Value &request, Json::Value &response)
	{
		std::stringstream ss;
		std::string Command = request["action"].asString();

		//ss << "OnRequest: " << Command << " ";
		//printf("%s\n", ss.str().c_str());

		if (Command == "PING")
			return 0;

		if (Command == "QUIT")
		{
			m_Quit = true;
			return 0;
		}

		
		return -ENOSYS;
	}
	
	int OnCommand(IServerConnection *Connection, Json::Value &request)
	{
		printf("OnCommand: %s\n", request["action"].asString().c_str());
		return -ENOSYS;
	}
	
	int OnJsonRequest(IServerConnection *Connection, Json::Value &req, Json::Value &res)
	{
		printf("OnJsonRequest\n");
		return -ENOSYS;
	}
	
	int OnJsonCommand(IServerConnection *Connection, Json::Value &req)
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
	        Manager.SetThreads(4);
			ServerUnix Unix("/tmp/TestServer");
			Manager.ServerAdd(&Unix);
			while(m_quit == false)
			{	Json::Value event;
				event["action"] = "RUNNING";
				Manager.SendEvent(event);
				sleep(1);
			}

			Manager.ServerRemove(&Unix);
		}
	private:
		bool m_quit;
};

class TestClient : Thread {
    public:
        TestClient(ClientBase *cli) {
            m_cli = cli;
            m_quit = false;
            Start();
        }

        ~TestClient() {
            m_quit = true;
			Stop();
        }

        int Ping(ClientBase *Client) {
            PerfCounter PC("PING");
            Json::Value request;
            Json::Value response;

            request["action"] = "PING";
            return Client->SendRequest(request, response);
        }

        virtual void Run() {
            while(m_quit == false) {
                if (Ping(m_cli) != 0)
                    retval = 1;
            }
        }

    private:
        ClientBase *m_cli;
        bool m_quit;
};

int main(int argc, char **argv)
{
	TestServer TS;
    ClientBase *Client = Client::Create("unix:/tmp/TestServer");
    do {
        Client->Connect();
        Client->WaitForConnect();

        std::list<std::shared_ptr<TestClient> > lst;
        lst.push_back(std::make_shared<TestClient>(Client));
        lst.push_back(std::make_shared<TestClient>(Client));
        lst.push_back(std::make_shared<TestClient>(Client));
        lst.push_back(std::make_shared<TestClient>(Client));
        for(int i =0;i<10000;i++)
        {
            Client->Disconnect();
            Client->Connect();
        }

        lst.clear();
        Client->Disconnect();
    } while(0);

	delete Client;

	return retval;
}

