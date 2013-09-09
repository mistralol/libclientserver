
class ClientBase
{
	public:
		virtual ~ClientBase() { };

		virtual void Connect() = 0;
		virtual void Disconnect() = 0;
		void WaitForConnect(); //Forever!
		bool WaitForConnect(const struct timespec *Timeout);

		void SetReConnectTimeout(const struct timespec *Timeout);
		void SetSoftTimeout(const struct timespec *SoftTimeout);
		void SetHardTimeout(const struct timespec *HardTimeout);

		void SendRequest(Request *request, Request *response, const struct timespec *SoftTimeout, const struct timespec *HardTimeout);
		void SendRequest(Request *request, Request *response, const struct timespec *SoftTimeout);
		void SendRequest(Request *request, Request *response);

		void SendCommand(Request *command);

	protected:
		void Init();
		virtual void DoSendRequest(Request *request, Request *response) = 0; //Should Never Block!
		virtual void DoSendCommand(Request *request) = 0; //Should never block!

		void RaiseOnConnect();
		void RaiseOnConnectError(int errno, const std::string str);
		void RaiseOnDisconnect(int errno, const std::string str); //With Error String?
		void RaiseOnResponse();	//Should be Raised from Helper
		void RaiseOnEvent(); //Should be Raised from helper

	protected:
		struct timespec m_ReConnectTimeout;
		struct timespec m_SoftTimeout;
		struct timespec m_HardTimeout;

};

