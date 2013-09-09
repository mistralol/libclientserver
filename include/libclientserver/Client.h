

class Client
{
	public:
		static ClientBase *Create(const std::string &ConnectionString);
		
		static ClientBase *CreateUnix(const std::string &ConnectionString);
		static ClientBase *CreateTcp(const std::string &ConnectionString);

};

