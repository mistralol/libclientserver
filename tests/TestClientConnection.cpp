
#include <libclientserver.h>

#include <TestSetup.h>
#include <TestServer.h>

int main(int argc, char **argv)
{
	TestSetup();
	TestServer TS;
	ClientBase *Client = Client::Create("unix:/tmp/TestServer");

	for(int i =0;i<1000;i++)
	{
		Client->Connect();
		Client->WaitForConnect();
		Client->Disconnect();
	}

	delete Client;
	return 0;
}

