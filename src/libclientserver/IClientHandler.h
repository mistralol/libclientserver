
/**
 * @IClientHandler
 * @brief An Interface for certain events that can be raised by the client
 *
 * Callback interface that is set on the client side.
 * This will provide various notifications to the host
 * application about certain state transitions when they occur.
 *
 * Such events will be like. OnConnect / OnDisconnect / Events.
 * and various others regarding communication issues.
 */

class IClientHandler
{
	public:
		virtual ~IClientHandler() { }
	
		virtual void OnConnect() {}
		virtual void OnConnectError(int err, const std::string &str) {}
		virtual void OnSendError(int err, const std::string &str) {}
		virtual void OnDisconnect(int err, const std::string &str) {}

		/* Permit the Client Implementation of Override these by returning false */
		/* Or just use them for logging purposes */
		virtual bool OnResponse(Json::Value &response) { return true; }
		virtual bool OnKeepAlive(Json::Value &response) { return true; }
		virtual bool OnEvent(Json::Value &event) { return true; }

		virtual void OnBadLine(const std::string *str) { abort(); }

};

