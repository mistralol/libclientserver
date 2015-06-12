
class GroupID
{
	public:
		GroupID();
		~GroupID();

		int Up(uid_t id);
		int Down();

	private:
		std::stack<uid_t> m_stack;

};

