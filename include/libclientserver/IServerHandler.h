
class IServerHandler {
	public:
		virtual ~IServerHandler() { }

		virtual void OnPreNewConnection() = 0;
		
		virtual void OnPostNewConnection() = 0;
		
		virtual void OnDisconnect() = 0;
		
		virtual void OnRequest(Request &request, Request &response) = 0;

};


