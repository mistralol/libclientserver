
class Request
{
	public:
		Request();
		~Request();

		void SetCommand(const std::string &Command);
		std::string GetCommand();

		bool HasArg(const std::string &Name);

		//Encode / Decode from a line of text
		//IDataType get / set with key
//		std::string Encode();
//		void Decode();

	protected:
		void Clear();

	private:
		std::string m_command;
		std::map<std::string, std::string> m_args;	
};

