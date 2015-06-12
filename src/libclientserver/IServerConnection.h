
/** 
 * @class IServerConnection
 * @brief Interface for Server Connections
 * 
 * Interface that every server connection must implement.
 */


class IServerConnection
{
	public:
		virtual ~IServerConnection() { }

		virtual bool SendLine(const std::string *str) = 0;

		virtual void SendEvent(Request *event)
		{
			std::string str = event->Encode();
			str = "EVENT " + str + "\n";
			SendLine(&str);
		}

		void *m_private;
};

