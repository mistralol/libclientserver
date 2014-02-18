

#include <libclientserver.h>

bool Ping(ClientBase *Client)
{
	struct timespec ts;
	Request request;
	Request response;

	clock_gettime(CLOCK_MONOTONIC, &ts);

	request.SetCommand("PING");
	request.SetArg("TIME", Encoder::ToStr(&ts));

	if (Client->SendRequest(&request, &response) == false)
		return false;

	return true;
}

bool TestCommand(ClientBase *Client)
{
	Request command;

	command.SetCommand("PRINT");

	return Client->SendCommand(&command);
}

int main(int argc, char **argv)
{
	ClientBase *Client = Client::Create("unix:/tmp/SimpleServer");

	Client->Connect();
	printf("Connecting\n");
	Client->WaitForConnect();
	printf("Connected\n");

	for(int i=0;i<3;i++)
	{
		printf("Sending Ping\n");
		if (Ping(Client) == false)
			printf("Failed\n");
	}

	printf("Disconnecting\n");
	Client->Disconnect();
	printf("DisConnected\n");
	delete Client;

	printf("CleanExit\n");
	return 0;
}

