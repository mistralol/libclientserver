
#include <libclientserver.h>

/**
 * Sleep
 * @param[in] Timeout The total length of time to sleep for
 */
void Time::Sleep(const struct timespec *Timeout)
{
	struct timespec ts;
	struct timespec rem;
	memcpy(&ts, Timeout, sizeof(ts));

restart:
	int ret = nanosleep(&ts, &rem);
	if (ret < 0)
	{
		switch(errno)
		{
			case EINTR:
				memcpy(&ts, &rem, sizeof(ts));
				goto restart;
				break;
			default:
				abort();
				break;
		}
	}
}

/**
 * Sleep
 * @param[in] Timeout The total length of time to sleep for
 */
void Time::Sleep(const struct timeval *Timeout)
{
	struct timespec ts;
	TimeValtoTimeSpec(Timeout, &ts);
	Sleep(&ts);
}

/**
 * TimeSpecToTimeVal
 * @param[in] ts Source Value
 * @param[out] tv Output Value
 *
 * Convert the input timespec to a timeval structure.
 */
void Time::TimeSpecToTimeVal(const struct timespec *ts, struct timeval *tv)
{
	tv->tv_sec = ts->tv_sec;
	tv->tv_usec = ts->tv_nsec / 1000;
}

/**
 * TimeValtoTimeSpec
 * @param[in] tv Source Value
 * @param[out] ts Output Value
 *
 * Convert the input timeval to a timespec structure.
 */
void Time::TimeValtoTimeSpec(const struct timeval *tv, struct timespec *ts)
{
	ts->tv_sec = tv->tv_sec;
	ts->tv_nsec = tv->tv_usec * 1000;
}

void Time::TimeSpecFromNanoSeconds(unsigned long long val, struct timespec *ts)
{
	ts->tv_sec = val / 1000000000L;
	ts->tv_nsec = val % 1000000000L;
}

void Time::TimeSpecFromMicroSeconds(unsigned long long val, struct timespec *ts)
{
	TimeSpecFromNanoSeconds(val * 1000, ts);
}

void Time::TimeSpecFromMilliSeconds(unsigned long long val, struct timespec *ts)
{
	TimeSpecFromMicroSeconds(val * 1000, ts);
}

void Time::TimeSpecFromSeconds(unsigned long long val, struct timespec *ts)
{
	TimeSpecFromMilliSeconds(val * 1000, ts);
}

void Time::TimeSpecFromMinutes(unsigned long long val, struct timespec *ts)
{
	TimeSpecFromSeconds(val * 60, ts);
}

void Time::TimeSpecFromHours(unsigned long long val, struct timespec *ts)
{
	TimeSpecFromMinutes(val * 60, ts);
}

void Time::TimeSpecFromDays(unsigned long long val, struct timespec *ts)
{
	TimeSpecFromHours(val * 24, ts);
}

void Time::TimeValFromNanoSeconds(unsigned long long val, struct timeval *ts)
{
	TimeValFromMicroSeconds(val / 1000, ts);
}

void Time::TimeValFromMicroSeconds(unsigned long long val, struct timeval *ts)
{
	ts->tv_sec = val / 1000000;
	ts->tv_usec = val % 1000000;
}

void Time::TimeValFromMilliSeconds(unsigned long long val, struct timeval *ts)
{
	TimeValFromMicroSeconds(val * 1000, ts);
}

void Time::TimeValFromSeconds(unsigned long long val, struct timeval *ts)
{
	TimeValFromMilliSeconds(val * 1000, ts);
}

void Time::TimeValFromMinutes(unsigned long long val, struct timeval *ts)
{
	TimeValFromSeconds(val * 60, ts);
}

void Time::TimeValFromHours(unsigned long long val, struct timeval *ts)
{
	TimeValFromMinutes(val * 60, ts);
}

void Time::TimeValFromDays(unsigned long long val, struct timeval *ts)
{
	TimeValFromHours(val * 24, ts);
}


unsigned long long Time::NanoSeconds(const struct timespec *ts)
{
	unsigned long long result = ts->tv_sec * 1000000000L;
	result += ts->tv_nsec;
	return result;
}

unsigned long long Time::NanoSeconds(const struct timeval *ts)
{
	unsigned long long result = ts->tv_sec * 1000000000L;
	result += (ts->tv_usec * 1000L);
	return result;
}

unsigned long long Time::Microseconds(const struct timespec *ts)
{
	return NanoSeconds(ts) / 1000;
}

unsigned long long Time::Microseconds(const struct timeval *ts)
{
	return NanoSeconds(ts) / 1000;
}

unsigned long long Time::MilliSeconds(const struct timespec *ts)
{
	return NanoSeconds(ts) / 1000000;
}

unsigned long long Time::MilliSeconds(const struct timeval *ts)
{
	return NanoSeconds(ts) / 1000000;
}

unsigned long long Time::Seconds(const struct timespec *ts)
{
	return MilliSeconds(ts) / 1000;
}

unsigned long long Time::Seconds(const struct timeval *ts)
{
	return MilliSeconds(ts) / 1000;
}

unsigned long long Time::Minutes(const struct timespec *ts)
{
	return Seconds(ts) / 60;
}

unsigned long long Time::Minutes(const struct timeval *ts)
{
	return Seconds(ts) / 60;
}

unsigned long long Time::Hours(const struct timespec *ts)
{
	return Minutes(ts) / 60;
}

unsigned long long Time::Hours(const struct timeval *ts)
{
	return Minutes(ts) / 60;
}

unsigned long long Time::Days(const struct timespec *ts)
{
	return Hours(ts) / 24;
}

unsigned long long Time::Days(const struct timeval *ts)
{
	return Hours(ts) / 24;
}

/**
 * Add
 * @param[in] ts1 First Input Value
 * @param[in] ts2 Second Input Value
 * @param[out] res Output Value
 *
 * Add two timespec struct together to form the output.
 * It is safe to have the output specificed as the same address as either of the two input structures.
 */
void Time::Add(const struct timespec *ts1, const struct timespec *ts2, struct timespec *res)
{
	res->tv_sec = ts1->tv_sec + ts2->tv_sec;
	res->tv_nsec = ts1->tv_nsec + ts2->tv_nsec;
	if (res->tv_nsec > 999999999)
	{
		res->tv_sec++;
		res->tv_nsec -= 1000000000;
	}
}

/**
 * Add
 * @param[in] tv1 First Input Value
 * @param[in] tv2 Second Input Value
 * @param[out] res Output Value
 *
 * Add two timeval struct together to form the output.
 * It is safe to have the output specificed as the same address as either of the two input structures.
 */
void Time::Add(const struct timeval *tv1, const struct timeval *tv2, struct timeval *res)
{
	res->tv_sec = tv1->tv_sec + tv2->tv_sec;
	res->tv_usec = tv1->tv_usec + tv2->tv_usec;
	if (res->tv_usec > 999999)
	{
		res->tv_sec++;
		res->tv_usec -= 1000000;
	}
}

/**
 * Sub
 * @param[in] ts1 First Input Value
 * @param[in] ts2 Second Input Value
 * @param[out] res Output Value
 *
 * Subtrace two timespec struct together to form the output.
 * It is safe to have the output specificed as the same address as either of the two input structures.
 */
void Time::Sub(const struct timespec *ts1, const struct timespec *ts2, struct timespec *res)
{
	res->tv_sec = ts1->tv_sec - ts2->tv_sec;
	res->tv_nsec = ts1->tv_nsec - ts2->tv_nsec;
	if (res->tv_nsec < 0)
	{
		res->tv_sec--;
		res->tv_nsec += 1000000000;
	}
}

/**
 * Sub
 * @param[in] tv1 First Input Value
 * @param[in] tv2 Second Input Value
 * @param[out] res Output Value
 *
 * Subtract two timeval struct together to form the output.
 * It is safe to have the output specificed as the same address as either of the two input structures.
 */
void Time::Sub(const struct timeval *tv1, const struct timeval *tv2, struct timeval *res)
{
	res->tv_sec = tv1->tv_sec - tv2->tv_sec;
	res->tv_usec = tv1->tv_usec - tv2->tv_usec;
	if (res->tv_usec < 0)
	{
		res->tv_sec--;
		res->tv_usec += 1000000;
	}
}

/**
 * Divide
 * @param[in] ts The input value
 * @param[in] div The value to divide by
 * @param[out] res The output values
 *
 * This function will divide the ts by the div paramaters.
 * It is permitted that the input and output paramaters can point to the same data item.
 */
void Time::Divide(const struct timespec *ts, int div, struct timespec *res)
{
	unsigned long long tmp = NanoSeconds(ts);
	tmp /= div;
	TimeSpecFromNanoSeconds(tmp, res);
}

/**
 * Divide
 * @param[in] ts The input value
 * @param[in] div The value to divide by
 * @param[out] res The output values
 *
 * This function will divide the ts by the div paramaters.
 * It is permitted that the input and output paramaters can point to the same data item.
 */
void Time::Divide(const struct timeval *ts, int div, struct timeval *res)
{
	unsigned long long tmp = NanoSeconds(ts);
	tmp /= div;
	TimeValFromNanoSeconds(tmp, res);
}

/**
 * Multiply
 * @param[in] ts The input value
 * @param[in] div The value to divide by
 * @param[out] res The output values
 *
 * This function will multiply the ts by the x paramaters.
 * It is permitted that the input and output paramaters can point to the same data item.
 */
void Time::Multiply(const struct timespec *ts, int x, struct timespec *res)
{
	unsigned long long tmp = NanoSeconds(ts);
	tmp *= x;
	TimeSpecFromNanoSeconds(tmp, res);
}

/**
 * Multiply
 * @param[in] ts The input value
 * @param[in] div The value to divide by
 * @param[out] res The output values
 *
 * This function will multiply the ts by the x paramaters.
 * It is permitted that the input and output paramaters can point to the same data item.
 */
void Time::Multiply(const struct timeval *ts, int x, struct timeval *res)
{
	unsigned long long tmp = NanoSeconds(ts);
	tmp *= x;
	TimeValFromNanoSeconds(tmp, res);
}


/**
 * Diff
 * @param[in] ts1 Input One
 * @param[in] ts2 Input Two
 * @param[out] res Output the difference
 *
 * This will calculate the difference between two times. The value outputted will always be a positive value regardless of the order of ts1 and ts2
 */
void Time::Diff(const struct timespec *ts1, const struct timespec *ts2, struct timespec *res)
{
	if (IsGreater(ts1, ts2))
	{
		Sub(ts2, ts1, res);
	}
	else
	{
		Sub(ts2, ts1, res);
	}
}

/**
 * Diff
 * @param[in] tv1 Input One
 * @param[in] tv2 Input Two
 * @param[out] res Output the difference
 *
 * This will calculate the difference between two times. The value outputted will always be a positive value regardless of the order of ts1 and ts2
 */
void Time::Diff(const struct timeval *tv1, const struct timeval *tv2, struct timeval *res)
{
	if (IsGreater(tv1, tv2))
	{
		Sub(tv2, tv1, res);
	}
	else
	{
		Sub(tv2, tv1, res);
	}
}

/**
 * IsLess
 * @param[in] ts1
 * @param[in] ts2
 * @return True if ts1 is less than ts2
 *
 * Returns true if the values in ts1 is less than the value in ts2
 */
bool Time::IsLess(const struct timespec *ts1, const struct timespec *ts2)
{
	if (ts1->tv_sec < ts2->tv_sec)
		return true;
	if (ts1->tv_sec == ts2->tv_sec && ts1->tv_nsec < ts2->tv_nsec)
		return true;
	return false;
}

/**
 * IsLess
 * @param[in] tv1
 * @param[in] tv2
 * @return True if tv1 is less than tv2
 *
 * Returns true if the values in tv1 is less than the value in tv2
 */
bool Time::IsLess(const struct timeval *tv1, const struct timeval *tv2)
{
	if (tv1->tv_sec < tv2->tv_sec)
		return true;
	if (tv1->tv_sec == tv2->tv_sec && tv1->tv_usec < tv2->tv_usec)
		return true;
	return false;
}

/**
 * IsGreater
 * @param[in] ts1
 * @param[in] ts2
 * @return True if ts1 is greater than ts2
 *
 * Returns true if the values in ts1 is greater than the value in ts2
 */
bool Time::IsGreater(const struct timespec *ts1, const struct timespec *ts2)
{
	if (ts1->tv_sec > ts2->tv_sec)
		return true;
	if (ts1->tv_sec == ts2->tv_sec && ts1->tv_nsec > ts2->tv_nsec)
		return true;
	return false;
}

/**
 * IsGreater
 * @param[in] tv1
 * @param[in] tv2
 * @return True if tv1 is greater than tv2
 *
 * Returns true if the values in tv1 is greater than the value in tv2
 */
bool Time::IsGreater(const struct timeval *tv1, const struct timeval *tv2)
{
	if (tv1->tv_sec > tv2->tv_sec)
		return true;
	if (tv1->tv_sec == tv2->tv_sec && tv1->tv_usec > tv2->tv_usec)
		return true;
	return false;

}

/**
 * Max
 * @param[in] ts
 *
 * Fill the timespec struct with the maximum possible time value
 */ 
void Time::Max(struct timespec *ts)
{
	ts->tv_sec = LONG_MAX;
	ts->tv_nsec = LONG_MAX;
}

/**
 * Max
 * @param[in] ts
 *
 * Fill the timeval struct with the maximum possible time value
 */ 
void Time::Max(struct timeval *tv)
{
	tv->tv_sec = LONG_MAX;
	tv->tv_usec = LONG_MAX;
}

/**
 * Max
 * @param[in] ts
 *
 * Fill the timeval struct with the minimum possible time value
 */ 
void Time::Zero(struct timespec *ts)
{
	ts->tv_sec = 0;
	ts->tv_nsec = 0;
}

/**
 * Max
 * @param[in] ts
 *
 * Fill the timeval struct with the minimum possible time value
 */ 
void Time::Zero(struct timeval *tv)
{
	tv->tv_sec = 0;
	tv->tv_usec = 0;
}

/*
 * MonoTonic
 * @param[out] ts The output value
 *
 * This will store a time from the monotonic wall clock in ts
 */
void Time::MonoTonic(struct timespec *ts)
{
	if (clock_gettime(CLOCK_MONOTONIC, ts) < 0)
		abort();
}

/*
 * MonoTonic
 * @param[out] ts The output value
 *
 * This will store a time from the monotonic wall clock in ts
 */
void Time::MonoTonic(struct timeval *tv)
{
	struct timespec ts;
	MonoTonic(&ts);
	TimeSpecToTimeVal(&ts, tv);
}

