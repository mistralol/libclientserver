
/**
 * @class ServerManager
 * @brief Manager class for tracking and controlling various methods of communication
 *
 * This class tracks / controls instances of various inbound communication methods.
 * It will also provides a means to pass through various server errors that can occur
 * if certain clients decide to send poor information.
 *
 * The events in this class are only passed onto the IServerHandler class.
 * But can be raised from many different connection / server types.
 * 
 *
 */

class ServerManager
{
	public:
		ServerManager(IServerHandler *);
		~ServerManager();
	
		void ServerAdd(IServer *Server);
		void ServerRemove(IServer *Server);
		void ServerRemoveAll();

		bool ProcessLine(IServerConnection *Connection, const std::string *line);

		void RaisePreNewConnection();
		void RaisePostNewConnection(IServerConnection *Connection);
		void RaiseDisconnect(IServerConnection *Connection);

		void RaiseBadLine(IServerConnection *Connection, const std::string *line);
	
		int RaiseRequest(IServerConnection *Connection, Json::Value &req, Json::Value &res);
		int RaiseCommand(IServerConnection *Connection, Json::Value &req);

		void SendEvent(Json::Value &event);
		int SendEvent(uint64_t ConnID, Json::Value &event);

	protected:

		void ConnectionAdd(IServerConnection *Conn);
		void ConnectionRemove(IServerConnection *Conn);

	
	private:
		IServerHandler *m_handler;
		std::list<IServer *> m_Servers;
		Mutex m_ServersMutex;

		//Connection Tracking
		Mutex m_ConnectionMutex;
		std::map<uint64_t, IServerConnection *> m_ConnectionMap;
		uint64_t m_ConnectionNextID; //Next number to assignt o a connection

		//Stats
		uint64_t m_TotalRequests;
		uint64_t m_TotalCommands;
};


