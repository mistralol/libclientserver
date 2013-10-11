
class ClientUnix : public ClientBase, Thread
{
	public:
		ClientUnix(const std::string path);
		virtual ~ClientUnix();

		void Connect();
		bool IsConnected();
		void Disconnect();

	protected:
		bool DoSendRequest(Request *request, const struct timespec *Timeout);
		bool DoSendCommand(Request *command, const struct timespec *Timeout);

	private:
		bool SendLine(const std::string *str, const struct timespec *Timeout);
		void Run();


	private:
		volatile bool m_quit;
		std::string m_path;
		int m_fd;
		volatile bool m_connected;

		RWLock m_WriterLock;
		Mutex m_WriterMutex;
};

