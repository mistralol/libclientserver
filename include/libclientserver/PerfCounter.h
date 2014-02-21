

/*
 * @class PerfManager
 * @brief Performance Counter class to track capture performance data
 *
 * Performance Manager class to track Performance data. 
 * This counter will also automatically add the data captured to the Performance Manager class
 */

class PerfCounter
{
	public:
		PerfCounter(const std::string Key);
		~PerfCounter();

	private:
		struct timespec m_starttime;
		std::string m_Key;
};


