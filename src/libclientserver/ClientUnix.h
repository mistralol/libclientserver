
/**
 * @class ClientUnix
 * @brief Implementation for Unix Sockets
 *
 * Provides client side implementation using unix sockets.
 */

class ClientUnix : public ClientBase, Thread
{
	public:
		ClientUnix(const std::string path);
		virtual ~ClientUnix();

		void Connect();
		bool IsConnected();
		void Disconnect();

	private:
		bool SendLine(const std::string *str, const struct timespec *Timeout);
		void Run();


	private:
		volatile bool m_quit;
		std::string m_path;
		int m_fd;

		RWLock m_WriterLock;
		Mutex m_WriterMutex;
};

