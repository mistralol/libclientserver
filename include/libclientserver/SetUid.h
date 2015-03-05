

class SetUid
{
	public:
		SetUid();
		~SetUid();

		int Init();

		uid_t GetUid();

	private:
		uid_t m_uid;
};


