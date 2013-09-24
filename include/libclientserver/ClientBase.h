
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

		virtual bool DoSendRequest(Request *request, const struct timespec *SoftTimeout) = 0; //Should Never Block!
		virtual bool DoSendCommand(Request *request, const struct timespec *SoftTimeout) = 0; //Should never block!

		void RaiseOnConnect();
		void RaiseOnConnectError(int err, const std::string &str);
		void RaiseOnDisconnect(int err, const std::string &str); //With Error String?
		void RaiseOnResponse();
		void RaiseOnEvent();
		void RaiseOnData(const std::string *str);

	protected:
		struct timespec m_ReConnectTimeout;
		struct timespec m_SoftTimeout;
		struct timespec m_HardTimeout;

		Mutex m_ConnectedMutex;

		Mutex m_LastIDMutex;
		uint64_t m_LastID;
		
		IClientHandler *m_Handler;

};

