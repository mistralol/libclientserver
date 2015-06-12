

/**
 * @class Client
 * @brief Factor class for being able to generate connections
 *
 * This class will create connections using the common interface ClientBase
 * which is based on connections strings. The connections should be completly
 * abstracted from the parent classes other than this entry point.
 */

class Client
{
	public:
		static ClientBase *Create(const std::string &ConnectionString);
		
		static ClientBase *CreateUnix(const std::string &ConnectionString);
		static ClientBase *CreateTcp(const std::string &ConnectionString);

};

