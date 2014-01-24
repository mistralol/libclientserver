

class IServerConnection
{
	public:
		virtual ~IServerConnection() { }

		virtual bool SendLine(const std::string *str) = 0;
};

