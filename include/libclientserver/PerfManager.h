

struct PerfDetails_t
{
	struct timespec TotalTime;
	struct timespec WorstTime;
	struct timespec BestTime;
	int Count;

	void Init()
	{
		TotalTime.tv_sec = 0;
		TotalTime.tv_nsec = 0;
		Count = 0;

		Time::Max(&BestTime);
		Time::Zero(&WorstTime);
	}
};

/*
 * @class PerfManager
 * @brief Performance Manager class to track Performance Counter Data
 *
 * Performance Manager class to track Performance Counter Data
 */

class PerfManager
{
	public:
		static std::list<std::string> GetKeyList();
		static void Clear(const std::string Key);
		static void Clear();

		static bool GetInfo(const std::string Key, struct PerfDetails_t *pd);
		static void UpdateCounter(const std::string Key, const struct timespec *ts);

		static void Dump();

	private:
		static std::map<std::string, struct PerfDetails_t> m_map;
		static Mutex m_Mutex;
};


