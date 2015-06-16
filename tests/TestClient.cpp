
#include <libclientserver.h>

#include <TestServer.h>

int main(int argc, char **argv)
{
	TestServer TS;
	ClientBase *Client = Client::Create("unix:/tmp/TestServer");

	Client->Connect();
	Client->WaitForConnect();
	Client->Disconnect();

	delete Client;
	return 0;
}

