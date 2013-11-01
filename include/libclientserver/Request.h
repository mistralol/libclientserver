
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
		std::string GetArg(const std::string *Key);
		void SetArg(const std::string *Key, const std::string *Value);
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

