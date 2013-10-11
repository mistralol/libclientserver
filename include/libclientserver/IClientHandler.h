

class IClientHandler
{
	public:
		virtual ~IClientHandler();
	
		virtual void OnConnect() {}
		virtual void OnConnectError(int err, const std::string &str) {}
		virtual void OnSendError(int err, const std::string &str) {}
		virtual void OnDisconnect(int err, const std::string &str) {}
		virtual void OnResponse() {}
		virtual void OnEvent() {}

};

