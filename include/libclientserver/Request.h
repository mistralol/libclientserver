
/**
 * @class Request
 * @brief Serializable class for Sending / Processing Requests
 *
 * This class is implemented to be serializable. It support a command name / sequence number
 * and an unlimited number of key / value pairs which will be encoded to a line of text.
 * 
 * It will also support decoding the same line of text.
 */

class Request
{
	public:
		Request();
		~Request();

		uint64_t GetID();
		void SetID(uint64_t ID);

		void SetCommand(const std::string &Command);
		std::string GetCommand();

		bool HasArg(const std::string &Name);
		std::list<std::string> GetArgList();
		std::string GetArg(const std::string *Key);
		std::string GetArg(const std::string Key);
		int GetInt(const std::string Key);
		void SetArg(const std::string *Key, const std::string *Value);
		void SetArg(const std::string Key, const std::string Value);
		void SetArg(const std::string Key, const std::string *Value);
		void SetArg(const std::string Key, int Value);
		void RemoveArg(const std::string *Key);

		std::string Encode();
		bool Decode(const std::string *str);

	protected:
		void Clear();

	private:
		uint64_t m_id;
		std::string m_command;
		std::map<std::string, std::string> m_args;	
};

