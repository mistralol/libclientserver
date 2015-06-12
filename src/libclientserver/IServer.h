
class ServerManager;
class IServerConnection;

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

		void ConnectionAdd(IServerConnection *Connection);
		void ConnectionRemove(IServerConnection *Connection);
		void ConnectionRemoveAll();

		void SendEvent(Request *event);

	private:
		std::list<IServerConnection *> m_Connections;
		Mutex m_ConnectionsMutex;

};



