
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

		virtual void SendEvent(const std::string &event)
		{
			SendLine(&event);
		}

		void *m_private;
};

