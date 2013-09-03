
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
		abort();
	}

	void OnPostNewConnection(IServerConnection *Connection)
	{
		abort();
	}
		
	void OnDisconnect(IServerConnection *Connection)
	{
		abort();
	}
		
	void OnRequest(IServerConnection *Connection, Request *request, Request *response)
	{
		abort();	
	}
	
	void OnCommand(IServerConnection *Connection, Request *request)
	{
		abort();	
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


int main(int argc, char **argv)
{
	Handler SrvHandler;
	ServerManager Manager(&SrvHandler);
	
	ServerUnix Unix("/tmp/SimpleServer");
	
	Manager.ServerAdd(&Unix);
	SrvHandler.Wait();
	Manager.ServerRemove(&Unix);
	
	return 0;
}



