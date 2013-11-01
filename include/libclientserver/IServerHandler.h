
class IServerHandler {
	public:
		virtual ~IServerHandler() { }

		virtual void OnPreNewConnection() = 0;
		virtual void OnPostNewConnection(IServerConnection *Connection) = 0;
		
		virtual void OnDisconnect(IServerConnection *Connection) = 0;
		
		virtual bool OnRequest(IServerConnection *Connection, Request *request, Request *response) = 0;
		virtual bool OnCommand(IServerConnection *Connection, Request *request) = 0;

		virtual void OnBadLine(IServerConnection *Connection, const std::string *line) = 0;
};


