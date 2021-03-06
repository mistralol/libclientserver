

/**
 * @class IServerHandler
 * @brief Main interface to drive the server from client command.
 *
 * A server will be required to impement this interface and add it to the ServerManager
 * This interface will drive all inbound events / commands / requests comming from clients.
 *
 */

class IServerHandler {
	public:
		virtual ~IServerHandler() { }

		virtual void OnPreNewConnection() = 0;
		virtual void OnPostNewConnection(IServerConnection *Connection) = 0;
		
		virtual void OnDisconnect(IServerConnection *Connection) = 0;
		
		virtual int OnRequest(IServerConnection *Connection, Json::Value &req, Json::Value &res) = 0;
		virtual int OnCommand(IServerConnection *Connection, Json::Value &req) = 0;

		virtual void OnBadLine(IServerConnection *Connection, const std::string *line) = 0;
};


