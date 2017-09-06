
/**
 * @class ClientUnix
 * @brief Implementation for Unix Sockets
 *
 * Provides client side implementation using unix sockets.
 */

class ClientTCP : public ClientBase, Thread
{
	public:
		ClientTCP(int port, const std::string &addr);
		virtual ~ClientTCP();

		void Connect();
		bool IsConnected();
		void Disconnect();

	private:
		bool SendLine(const std::string *str, const struct timespec *Timeout);
		int Connect4(struct sockaddr_in &addr);
		int Connect6(struct sockaddr_in6 &addr);
		void Run();


	private:
		volatile bool m_quit;
		int m_fd;
		int m_port;
		std::string m_addr;

		RWLock m_WriterLock;
		Mutex m_WriterMutex;
};

