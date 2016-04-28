
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
	
	private:
		IServerHandler *m_handler;
		std::list<IServer *> m_Servers;
		Mutex m_ServersMutex;

		uint64_t m_TotalRequests;
		uint64_t m_TotalCommands;

};



