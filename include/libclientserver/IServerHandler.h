
class IServerHandler {
	public:
		virtual ~IServerHandler() { }

		virtual void OnPreNewConnection() = 0;
		virtual void OnPostNewConnection(IServerConnection *Connection) = 0;
		
		virtual void OnDisconnect(IServerConnection *Connection) = 0;
		
		virtual void OnRequest(IServerConnection *Connection, Request *request, Request *response) = 0;
		virtual void OnCommand(IServerConnection *Connection, Request *request) = 0;
};


