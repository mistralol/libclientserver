
#include <libclientserver.h>

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

void Time::Sleep(const struct timeval *Timeout)
{
	struct timespec ts;
	TimeValtoTimeSpec(Timeout, &ts);
	Sleep(Timeout);
}

void Time::TimeSpecToTimeVal(const struct timespec *ts, struct timeval *tv)
{
	tv->tv_sec = ts->tv_sec;
	tv->tv_usec = ts->tv_nsec / 1000;
}

void Time::TimeValtoTimeSpec(const struct timeval *tv, struct timespec *ts)
{
	ts->tv_sec = tv->tv_sec;
	ts->tv_nsec = tv->tv_usec * 1000;
}

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

void Time::Sub(const struct timespec *ts1, const struct timespec *ts2, struct timespec *res)
{
	res->tv_sec = ts1->tv_sec - ts2->tv_sec;
	res->tv_nsec = ts2->tv_nsec - ts2->tv_nsec;
	if (res->tv_nsec < 0)
	{
		res->tv_sec--;
		res->tv_nsec += 1000000000;
	}
}

void Time::Sub(const struct timeval *tv1, const struct timeval *tv2, struct timeval *res)
{
	res->tv_sec = tv1->tv_sec - tv2->tv_sec;
	res->tv_usec = tv2->tv_usec - tv2->tv_usec;
	if (res->tv_usec < 0)
	{
		res->tv_sec--;
		res->tv_usec += 1000000;
	}
}

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

//if (ts1 < ts2)
bool Time::IsLess(const struct timespec *ts1, const struct timespec *ts2)
{
	if (ts1->tv_sec < ts2->tv_sec)
		return true;
	if (ts1->tv_sec == ts2->tv_sec && ts1->tv_nsec < ts2->tv_nsec)
		return true;
	return false;
}

bool Time::IsLess(const struct timeval *tv1, const struct timeval *tv2)
{
	if (tv1->tv_sec < tv2->tv_sec)
		return true;
	if (tv1->tv_sec == tv2->tv_sec && tv1->tv_usec < tv2->tv_usec)
		return true;
	return false;
}

//if (ts1 > ts2)
bool Time::IsGreater(const struct timespec *ts1, const struct timespec *ts2)
{
	if (ts1->tv_sec > ts2->tv_sec)
		return true;
	if (ts1->tv_sec == ts2->tv_sec && ts1->tv_nsec > ts2->tv_nsec)
		return true;
	return false;
}

bool Time::IsGreater(const struct timeval *tv1, const struct timeval *tv2)
{
	if (tv1->tv_sec > tv2->tv_sec)
		return true;
	if (tv1->tv_sec == tv2->tv_sec && tv1->tv_usec > tv2->tv_usec)
		return true;
	return false;

}


