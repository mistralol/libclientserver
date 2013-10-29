
/** @class Time
 * @brief Utility / Helper Functions for time operations on structures / functions
 * Provides various utility functions for helping to deal with time date / time structures and functions
 */

class Time
{
	public:
		static void Sleep(const struct timespec *Timeout);
		static void Sleep(const struct timeval *Timeout);

		static void TimeSpecToTimeVal(const struct timespec *ts, struct timeval *tv);
		static void TimeValtoTimeSpec(const struct timeval *tv, struct timespec *ts);

		static void Add(const struct timespec *ts1, const struct timespec *ts2, struct timespec *res);
		static void Add(const struct timeval *tv1, const struct timeval *tv2, struct timeval *res);

		static void Sub(const struct timespec *ts1, const struct timespec *ts2, struct timespec *res);
		static void Sub(const struct timeval *tv1, const struct timeval *tv2, struct timeval *res);

		static void Diff(const struct timespec *ts1, const struct timespec *ts2, struct timespec *res);
		static void Diff(const struct timeval *tv1, const struct timeval *tv2, struct timeval *res);

		static bool IsLess(const struct timespec *ts1, const struct timespec *ts2);
		static bool IsLess(const struct timeval *tv1, const struct timeval *tv2);

		static bool IsGreater(const struct timespec *ts1, const struct timespec *ts2);
		static bool IsGreater(const struct timeval *tv1, const struct timeval *tv2);
};

