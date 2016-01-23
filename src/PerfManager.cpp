
#include <libclientserver.h>

std::map<std::string, struct PerfDetails_t> PerfManager::m_map;
Mutex PerfManager::m_Mutex;


std::list<std::string> PerfManager::GetKeyList()
{
	std::list<std::string> lst;
	ScopedLock lock(&m_Mutex);
	for(std::map<std::string, struct PerfDetails_t>::iterator it = m_map.begin(); it != m_map.end(); it++)
	{
		lst.push_back(it->first);
	}
	return lst;
}

void PerfManager::Clear(const std::string Key)
{
	ScopedLock lock(&m_Mutex);
	std::map<std::string, struct PerfDetails_t>::iterator it = m_map.find(Key);
	if (it == m_map.end())
		return;
	m_map.erase(it);
}

void PerfManager::Clear()
{
	ScopedLock lock(&m_Mutex);
	m_map.clear();
}

bool PerfManager::GetInfo(const std::string Key, struct PerfDetails_t *pd)
{
	ScopedLock lock(&m_Mutex);
	std::map<std::string, struct PerfDetails_t>::iterator it = m_map.find(Key);
	if (it == m_map.end())
		return false;
	*pd = it->second;
	return true;
}

void PerfManager::UpdateCounter(const std::string Key, const struct timespec *ts)
{
	ScopedLock lock(&m_Mutex);
	std::map<std::string, struct PerfDetails_t>::iterator it = m_map.find(Key);
	if (it == m_map.end())
	{
		struct PerfDetails_t pd;
		pd.Init();
		m_map[Key] = pd;
		it = m_map.find(Key);
	}
	Time::Add(&it->second.TotalTime, ts, &it->second.TotalTime);
	if (Time::IsGreater(ts, &it->second.WorstTime))
		it->second.WorstTime = *ts;
	if (Time::IsLess(ts, &it->second.BestTime))
		it->second.BestTime = *ts;
	it->second.Count++;
}

void PerfManager::Dump()
{
	ScopedLock lock(&m_Mutex);

	for(std::map<std::string, struct PerfDetails_t>::iterator it = m_map.begin(); it != m_map.end(); it++)
	{
		printf("%15s %5d Calls %8ld Seconds %10ld NanoSeconds\n", it->first.c_str(), it->second.Count, it->second.TotalTime.tv_sec, it->second.TotalTime.tv_nsec);
	}
}

