
class Time
{
	public:
		static boost::system_time CalcTimeout(const struct timespec *Timeout);
		static boost::system_time CalcTimeout(const struct timeval *Timeout);
		
		static void Sleep(const struct timespec *Timeout);
};

