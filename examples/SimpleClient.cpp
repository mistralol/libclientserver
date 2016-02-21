

#include <libclientserver.h>

static int Ping(ClientBase *Client)
{
	PerfCounter PC("PING");
	Request request;
	Request response;

	request.SetCommand("PING");

	return Client->SendRequest(&request, &response);
}

static int Throw(ClientBase *Client)
{
	PerfCounter PC("Throw");
	Request request;
	Request response;
	request.SetCommand("THROW");
	return Client->SendRequest(&request, &response);
}

static int PingNoPerf(ClientBase *Client)
{
	Request request;
	Request response;

	request.SetCommand("PING");

	return Client->SendRequest(&request, &response);
}

#if 0
static int Quit(ClientBase *Client)
{
	Request request;
	Request response;

	request.SetCommand("QUIT");

	return Client->SendRequest(&request, &response);
}

static bool TestCommand(ClientBase *Client)
{
	Request command;

	command.SetCommand("PRINT");

	return Client->SendCommand(&command);
}
#endif

class Bench : public Thread
{
	public:
		Bench(ClientBase *cli, int count)
		{
			client = cli;
			m_count = count;
		}

		~Bench()
		{

		}

		void Run()
		{
			for(int i =0;i<m_count;i++)
			{
				int ret = PingNoPerf(client);
				if (ret < 0)
				{
					printf("BenchMark Failed: %d / %s i = %d\n", ret, strerror(abs(ret)), i);
				}
			}
		}
	private:
		int m_count;
		ClientBase *client;
	
};

int main(int argc, char **argv)
{
	ClientBase *Client = Client::Create("unix:/tmp/SimpleServer");
	bool Fail = false;

	Client->Connect();
	printf("Connecting\n");
	Client->WaitForConnect();
	printf("Connected\n");

	try
	{
		Throw(Client);
	} catch(ServerException &e)
	{
		printf("TestException: %s\n", e.what());
	}

	for(int i=0;i<5;i++)
	{
		printf("Sending Ping\n");
		int ret = Ping(Client);
		if (ret < 0)
		{
			printf("Failed: %d / %s\n", ret, strerror(ret));
			Fail = true;
		}
	}

	PerfManager::Dump();
	
	printf("Doing Bench Mark\n");
	std::list<Bench *> threads;
	
	for(int i = 1;i<=16;i++)
	{
		int count = 500000;
		for(int j=0;j<i;j++)
		{
			threads.push_back(new Bench(Client, count / i));
		}
		
		struct timespec tstart, tend;
		Time::MonoTonic(&tstart);
		
		for(std::list<Bench *>::iterator it = threads.begin(); it != threads.end(); it++)
		{
			Bench *b = *it;
			b->Start();
		}
		
		while(threads.size() > 0)
		{
			std::list<Bench *>::iterator it = threads.begin();
			Bench *b = *it;
			b->Stop();
			threads.erase(it);
			delete b;
		}
		
		Time::MonoTonic(&tend);
		
		struct timespec ttime, tsingle;
		Time::Sub(&tend, &tstart, &ttime);
		Time::Divide(&ttime, count, &tsingle);
		printf("Threads: %d TotalRequests: %d TotalTime: %" PRIu64 " (MS) Time Per Call: %" PRIu64 " (MS) OR %" PRIu64 " (NS)\n", i, count, Time::MilliSeconds(&ttime), Time::MilliSeconds(&tsingle), Time::NanoSeconds(&tsingle));

	}

/*
	if (Quit(Client) != 0)
	{
		printf("Server Failed To Quit!\n");
		Fail = true;
	}
*/

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

