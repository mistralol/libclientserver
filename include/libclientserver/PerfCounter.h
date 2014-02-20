
class PerfCounter
{
	public:
		PerfCounter(const std::string Key);
		~PerfCounter();

	private:
		struct timespec m_starttime;
		std::string m_Key;
};


