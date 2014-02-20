
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
	Sleep(Timeout);
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


