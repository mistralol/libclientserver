
/**
 * @class ClientUnixSelected
 * @brief Implementation for Unix Sockets
 *
 * Provides client side implementation using unix sockets.
 */

class ClientUnixSelected : public ClientBase
{
	public:
		ClientUnixSelected(const std::string path);
		virtual ~ClientUnixSelected();

		void Connect();
		bool IsConnected();
		void Disconnect();

                void DoRaiseOnConnect() { RaiseOnConnect(); }
                void DoRaiseOnConnectError(int err, const std::string &str) { RaiseOnConnectError(err, str); }
                void DoRaiseOnSendError(int err, const std::string &str) { RaiseOnSendError(err, str); }
                void DoRaiseOnDisconnect(int err, const std::string &str) { RaiseOnDisconnect(err, str); }
                void DoRaiseOnData(const std::string *str) { RaiseOnData(str); }
                
                void GetReconnectDelay(struct timespec *tv);

	private:
		bool SendLine(const std::string *str, const struct timespec *Timeout);

	private:
		std::string m_path;
		
		Selector *m_selector;
		ClientUnixSelectedConnection *m_connection;
};

