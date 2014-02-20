
#include <libclientserver.h>

PerfCounter::PerfCounter(const std::string Key)
{
	m_Key = Key;
	if (clock_gettime(CLOCK_MONOTONIC, &m_starttime) < 0)
		abort();
	printf("Start: %s\n", Key.c_str());
}

PerfCounter::~PerfCounter()
{
	struct timespec endtime;
	struct timespec diff;

	if (clock_gettime(CLOCK_MONOTONIC, &endtime) < 0)
		abort();

	Time::Diff(&m_starttime, &endtime, &diff);

	PerfManager::UpdateCounter(m_Key, &diff);
	printf("End: %s\n", m_Key.c_str());
}


