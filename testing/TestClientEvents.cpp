
#include <libclientserver.h>

#include <TestServer.h>

class CHandler : public IClientHandler
{
public:
	CHandler()
	{
		m_events = 0;
	}

	~CHandler()
	{

	}

	bool OnEvent(Request *event) {
		m_events++;
		return true;
	}

	bool ShouldExit()
	{
		if (m_events > 3)
			return true;
		return false;
	}

private:
	int m_events;

};

int main(int argc, char **argv)
{
	CHandler H;
	TestServer TS;
	ClientBase *Client = Client::Create("unix:/tmp/TestServer");

	Client->SetHandler(&H);

	Client->Connect();
	Client->WaitForConnect();

	while(H.ShouldExit() == false)
	{
		sleep(1);
	}

	Client->Disconnect();

	delete Client;
	return 0;
}

