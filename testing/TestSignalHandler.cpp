
#include <libclientserver.h>

bool loop = true;

class Handler : public ISignalHandler
{
	public:
		Handler() { };
		~Handler() { };

		void SigUser1(const siginfo_t *info)
		{
			loop = false;
		}
};


int main(int argc, char **argv)
{
	Handler H;
	SignalHandler S = SignalHandler(&H);

	if (kill(getpid(), SIGUSR1) < 0)
		abort();

	while(loop)
	{

	}

	return 0;
}
