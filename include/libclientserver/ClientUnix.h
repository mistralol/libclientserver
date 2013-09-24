
class ClientUnix : public ClientBase, Thread
{
	public:
		ClientUnix(const std::string path);
		virtual ~ClientUnix();

		void Connect();
		bool IsConnected();
		void Disconnect();

	protected:
		bool DoSendRequest(Request *request, const timespec *SoftTimeout);
		bool DoSendCommand(Request *command, const timespec *SoftTimeout);

	private:
		bool SendLine(const std::string *str, const timespec *SoftTimeout);
		void Run();


	private:
		volatile bool m_quit;
		std::string m_path;
		int m_fd;
		volatile bool m_connected;

		Mutex m_WriterMutex;
};

