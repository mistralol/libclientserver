
class UserID
{
	public:
		UserID();
		~UserID();

		int Up(uid_t id);
		int Down();

		static uid_t GetUIDFromName(const std::string username);
		static gid_t GetGIDFromName(const std::string username);

	private:
		std::stack<uid_t> m_stack;

};

