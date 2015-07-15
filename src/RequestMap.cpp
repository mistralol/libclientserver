

#include <libclientserver.h>

RequestMap::RequestMap()
{

}

RequestMap::~RequestMap()
{

}

void RequestMap::Add(struct RequestMapEntry *Entry)
{
	ScopedLock lock(&m_Mutex);
	uint64_t id = Entry->id;
	std::map<uint64_t, struct RequestMapEntry *>::iterator it = m_map.find(id);
	if (it != m_map.end())
		abort();	//Bug! Added duplicate

	m_map[id] = Entry;
}

void RequestMap::Remove(struct RequestMapEntry *Entry)
{
	ScopedLock lock(&m_Mutex);
	uint64_t id = Entry->id;
	std::map<uint64_t, struct RequestMapEntry *>::iterator it = m_map.find(id);
	if (it == m_map.end())
		abort();	//Bug! Attempted to remove something that does not exist

	m_map.erase(it);
}

bool RequestMap::Exists(struct RequestMapEntry *Entry)
{
	ScopedLock lock(&m_Mutex);
	uint64_t id = Entry->id;
	std::map<uint64_t, struct RequestMapEntry *>::iterator it = m_map.find(id);
	if (it == m_map.end())
		return false;
	return true;
}

bool RequestMap::Wait(struct RequestMapEntry *Entry, const struct timespec *SoftTimeout, const struct timespec *HardTimeout)
{
	ScopedLock lock(&m_Mutex);
	uint64_t id = Entry->id;
	std::map<uint64_t, struct RequestMapEntry *>::iterator it = m_map.find(id);
	if (it == m_map.end())
		return false; //Trying to wait on something that does not exist

	//FIXME: Current this is only precise to 1 second. It should be better!
	struct timespec CurrentTime;
	struct timespec DeadLine;
	struct timespec HardLine;
	struct timespec rem; //Remaiing time
	if (clock_gettime(CLOCK_MONOTONIC, &CurrentTime) < 0)
		abort();

	DeadLine.tv_sec = CurrentTime.tv_sec + SoftTimeout->tv_sec;
	DeadLine.tv_nsec = 0;

	HardLine.tv_sec = CurrentTime.tv_sec + HardTimeout->tv_sec;
	HardLine.tv_nsec = 0;

	while(Entry->ValidResponse == false)
	{
		if (clock_gettime(CLOCK_MONOTONIC, &CurrentTime) < 0)
			abort();

		//If we cross a dead line give up
		if (CurrentTime.tv_sec > DeadLine.tv_sec)
			return false;

		if (CurrentTime.tv_sec > HardLine.tv_sec)
			return false;

		//If keepalive is flagged
		if (Entry->KeepAlive == true)
		{
			Entry->KeepAlive = false;
			DeadLine.tv_sec = CurrentTime.tv_sec + SoftTimeout->tv_sec;
			DeadLine.tv_nsec = 0;
		}

		//Calculate the shortest sleeping time
		rem.tv_sec = DeadLine.tv_sec - CurrentTime.tv_sec;
		rem.tv_nsec = 0;

		//If the hard limit is smaller it overrules the soft limit
		if (HardLine.tv_sec - CurrentTime.tv_sec < rem.tv_sec)
			rem.tv_sec = HardLine.tv_sec - CurrentTime.tv_sec;

		m_Mutex.Wait(&rem); //Wait for WakeUpAll to be called
	}
	return true;
}

bool RequestMap::WakeUp(Request *response)
{
	ScopedLock lock(&m_Mutex);
	uint64_t id = response->GetID();
	std::map<uint64_t, struct RequestMapEntry *>::iterator it = m_map.find(id);
	if (it == m_map.end())
		return false; //Nothing left to wakeup. Probably a timeout and has been removed

	*it->second->Response = *response;
	it->second->ValidResponse = true;
	m_Mutex.WakeUpAll();
	return true;
}

bool RequestMap::KeepAlive(Request *response)
{
	ScopedLock lock(&m_Mutex);
	uint64_t id = response->GetID();
	std::map<uint64_t, struct RequestMapEntry *>::iterator it = m_map.find(id);
	if (it == m_map.end())
		return false;
	it->second->KeepAlive = true;
	return true;
}

