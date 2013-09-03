
class ServerManager;

class IServer
{
	public:
		virtual ~IServer() { }

		virtual void Start(ServerManager *Manager) = 0;
		virtual void Stop() = 0;

	protected:
		virtual void Run() = 0;

	private:

};



