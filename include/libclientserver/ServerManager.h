


class ServerManager
{
	public:
		ServerManager(IServerHandler *);
		~ServerManager();
	
		void ServerAdd(IServer *Server);
		void ServerRemove(IServer *Server);
		void ServerRemoveAll();
		
		void ConnectionAdd();
		void ConnectionRemove();
		void ConnectionRemoveAll();
		void ConnectionRemoveAll(IServer *Server);
		
	private:
		IServerHandler *m_handler;
		std::list<IServer *> m_Servers;
		
};



