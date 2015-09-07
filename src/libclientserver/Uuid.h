

class Uuid
{
	public:
		Uuid();
		Uuid(const std::string);
		~Uuid();
		
		std::string ToString();		
		static Uuid *TryParse(const std::string);
		
	private:
		uuid_t m_value;
};

