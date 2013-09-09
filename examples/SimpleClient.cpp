

#include <libclientserver.h>

int main(int argc, char **argv)
{
	ClientBase *Client = Client::Create("unix:/tmp/SimpleServer");

	Client->Connect();
	Client->WaitForConnect();

	Client->Disconnect();
	delete Client;
	return 0;
}

