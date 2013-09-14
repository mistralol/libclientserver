

class IClientHandler
{
	public:
		virtual ~IClientHandler();
	
		void OnConnect() {}
		void OnConnectError(int err, const std::string &str) {}
		void OnDisconnect(int err, const std::string &str) {}
		void OnResponse() {}
		void OnEvent() {}

};

