


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

		void ProcessLine(IServerConnection *Connection, const std::string *line);

		void RaisePreNewConnection();
		void RaisePostNewConnection(IServerConnection *Connection);
		void RaiseDisconnect(IServerConnection *Connection);
		void RaiseRequest(IServerConnection *Connection, Request *request, Request *response);
		void RaiseCommand(IServerConnection *Connection, Request *request);
	
	private:
		IServerHandler *m_handler;
		std::list<IServer *> m_Servers;
		Mutex m_ServersMutex;

		std::list<IServerConnection *> m_Connections;
		Mutex m_ConnectionsMutex;

};



