
class ClientUnix : public ClientBase, Thread
{
	public:
		ClientUnix(const std::string path);
		virtual ~ClientUnix();

		void Connect();
		bool IsConnected();
		void Disconnect();

	protected:
		bool DoSendRequest(Request *request, Request *response);
		bool DoSendCommand(Request *command);

	private:
		void Run();

	private:
		volatile bool m_quit;
		std::string m_path;
		int m_fd;
		volatile bool m_connected;
};

