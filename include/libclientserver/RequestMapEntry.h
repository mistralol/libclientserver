

class RequestMapEntry
{
	public:
		RequestMapEntry();
		~RequestMapEntry();

	private:
		uint64_t m_id;
		struct timespec m_starttime;
		Mutex m_mutex;
};

