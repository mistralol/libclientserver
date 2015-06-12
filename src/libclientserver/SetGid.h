

class SetGid
{
	public:
		SetGid();
		~SetGid();

		int Init();

		uid_t GetGid();

	private:
		uid_t m_gid;
};


