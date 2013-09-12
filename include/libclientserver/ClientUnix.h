
class ClientUnix : public ClientBase, Thread
{
	public:
		ClientUnix(const std::string path);
		virtual ~ClientUnix();

		void Connect();
		bool IsConnected();
		void Disconnect();

	protected:
		void DoSendRequest(Request *request, Request *response);
		void DoSendCommand(Request *command);

	private:
		void Run();

	private:
		bool m_quit;
		std::string m_path;
		int m_fd;
		bool m_connected;
};

