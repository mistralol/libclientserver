
/** 
 * @class ClientBase
 * @brief Main client interface class.
 * 
 * The main interface that should be used on the client side of the program.
 * It is expected that this class will normally be used by a wrapper to export
 * an api to the program wishing to communicate with the server.
 *
 * You should only every create an instance of this class from the factory Client::Create
 *
 */

class ClientBase
{
	public:
		virtual ~ClientBase() { };

		virtual void Connect() = 0;
		virtual bool IsConnected() = 0;
		virtual void Disconnect() = 0;

		void WaitForConnect(); //Forever!
		bool WaitForConnect(const struct timespec *Timeout);

		void SetReConnectTimeout(const struct timespec *Timeout);
		void SetSoftTimeout(const struct timespec *SoftTimeout);
		void SetHardTimeout(const struct timespec *HardTimeout);

		void SetHandler(IClientHandler *Handler);

		bool SendRequest(Request *request, Request *response, const struct timespec *SoftTimeout, const struct timespec *HardTimeout);
		bool SendRequest(Request *request, Request *response, const struct timespec *SoftTimeout);
		bool SendRequest(Request *request, Request *response);

		bool SendCommand(Request *command, const struct timespec *Timeout);
		bool SendCommand(Request *command);

		uint64_t GetNextID();
	protected:
		void Init();

		bool DoSendRequest(Request *request, const struct timespec *SoftTimeout);
		bool DoSendCommand(Request *request, const struct timespec *SoftTimeout);

		virtual bool SendLine(const std::string *str, const struct timespec *Timeout) = 0; //Should Never Block!

		void RaiseOnConnect();
		void RaiseOnConnectError(int err, const std::string &str);
		void RaiseOnSendError(int err, const std::string &str);
		void RaiseOnDisconnect(int err, const std::string &str); //With Error String?
		void RaiseOnResponse(Request *response);
		void RaiseOnKeepAlive(Request *response);
		void RaiseOnEvent(Request *event);
		void RaiseOnData(const std::string *str);
		void RaiseOnBadLine(const std::string *str);

	protected:
		struct timespec m_ReConnectTimeout;
		struct timespec m_SoftTimeout;
		struct timespec m_HardTimeout;

		Mutex m_ConnectedMutex;

		Mutex m_LastIDMutex;
		uint64_t m_LastID;

		RequestMap m_rmap;
		
		IClientHandler *m_Handler;

};

