

#include <libclientserver.h>

int Ping(ClientBase *Client)
{
	Request request;
	Request response;

	request.SetCommand("PING");

	return Client->SendRequest(&request, &response);
}

int Quit(ClientBase *Client)
{
	Request request;
	Request response;

	request.SetCommand("QUIT");

	return Client->SendRequest(&request, &response);
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
	bool Fail = false;

	Client->Connect();
	printf("Connecting\n");
	Client->WaitForConnect();
	printf("Connected\n");

	for(int i=0;i<3;i++)
	{
		printf("Sending Ping\n");
		int ret = Ping(Client);
		if (ret < 0)
		{
			printf("Failed: %d / %s\n", ret, strerror(ret));
			Fail = true;
		}
	}

	if (Quit(Client) != 0)
	{
		printf("Server Failed To Quit!\n");
		Fail = true;
	}

	printf("Disconnecting\n");
	Client->Disconnect();
	printf("DisConnected\n");
	delete Client;

	printf("CleanExit\n");

	if (Fail)
	{
		printf("Test: FAILED\n");
	}
	else
	{
		printf("Test: PASSED\n");
	}

	return 0;
}

