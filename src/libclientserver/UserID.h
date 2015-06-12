
class UserID
{
	public:
		UserID();
		~UserID();

		int Up(uid_t id);
		int Down();

	private:
		std::stack<uid_t> m_stack;

};

