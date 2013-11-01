


class ServerManager
{
	public:
		ServerManager(IServerHandler *);
		~ServerManager();
	
		void ServerAdd(IServer *Server);
		void ServerRemove(IServer *Server);
		void ServerRemoveAll();
		
		void ConnectionAdd(IServerConnection *Connection);
		void ConnectionRemove(IServerConnection *Connection);
		void ConnectionRemoveAll();
		void ConnectionRemoveAll(IServer *Server);

		bool ProcessLine(IServerConnection *Connection, const std::string *line);

		void RaisePreNewConnection();
		void RaisePostNewConnection(IServerConnection *Connection);
		void RaiseDisconnect(IServerConnection *Connection);

		void RaiseBadLine(IServerConnection *Connection, const std::string *line);
		bool RaiseRequest(IServerConnection *Connection, Request *request, Request *response);
		bool RaiseCommand(IServerConnection *Connection, Request *request);
	
	private:
		IServerHandler *m_handler;
		std::list<IServer *> m_Servers;
		Mutex m_ServersMutex;

		std::list<IServerConnection *> m_Connections;
		Mutex m_ConnectionsMutex;

		uint64_t m_TotalRequests;
		uint64_t m_TotalCommands;

};



