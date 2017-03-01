
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

		virtual void SendResponse(const std::string &response)
		{
			SendLine(&response);
		}

		uint64_t GetConnID() { return m_connid; };
		void SetConnID(uint64_t id) { m_connid = id; };

	private:
		uint64_t m_connid;

};

