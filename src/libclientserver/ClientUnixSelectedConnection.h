
class ClientUnixSelected;

class ClientUnixSelectedConnection : public ISelectable
{
	public:
		ClientUnixSelectedConnection(ClientUnixSelected *client, const std::string &path);
		~ClientUnixSelectedConnection();
		
		bool CanRead(const Selector *);
		bool CanWrite(const Selector *);
		bool CanExcept(const Selector *);
		bool CanTimeout(const Selector *);

		void DoRead(Selector *);
		void DoWrite(Selector *);
		void DoExcept(Selector *);
		void DoTimeout(Selector *);
		void DoClose(Selector *);

		int GetFD(const Selector *);
		void GetTimeout(const Selector *, struct timespec *tv);
		
		bool SendLine(Selector *, const std::string *);
		bool IsConnected();

	private:
		void NewSocket(Selector *p);
		void DoConnect();

	private:
		ClientUnixSelected *m_client;
		std::string m_path;
		int m_fd;
		bool m_first;
		bool m_failing;
		bool m_connected;
		Buffer m_rbuffer;
		Buffer m_wbuffer;
		Mutex m_buffermutex;
		
};

