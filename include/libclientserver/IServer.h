
class ServerManager;

/**
 * @class IServer
 * @brief Interface for a server implementation
 *
 * Interface to control the server. eg Start / Stop.
 */

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



