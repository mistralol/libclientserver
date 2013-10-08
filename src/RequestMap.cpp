

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

void RequestMap::Wait(struct RequestMapEntry *Entry, const struct timespec *SoftTimeout, const struct timespec *HardTimeout)
{
	ScopedLock lock(&m_Mutex);
	uint64_t id = Entry->id;
	std::map<uint64_t, struct RequestMapEntry *>::iterator it = m_map.find(id);
	if (it == m_map.end())
		abort(); //Trying to wait on something that does not exist

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
			return;

		if (CurrentTime.tv_sec > HardLine.tv_sec)
			return;

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
}

void RequestMap::WakeUp(uint64_t id, const std::string *str)
{
	ScopedLock lock(&m_Mutex);
	std::map<uint64_t, struct RequestMapEntry *>::iterator it = m_map.find(id);
	if (it == m_map.end())
		return;	//Nothing left to wakeup

	it->second->Response->Decode(str);
	it->second->ValidResponse = true;
	m_Mutex.WakeUpAll();
}

void RequestMap::KeepAlive(uint64_t id)
{
	ScopedLock lock(&m_Mutex);
	std::map<uint64_t, struct RequestMapEntry *>::iterator it = m_map.find(id);
	if (it == m_map.end())
		return;
	it->second->KeepAlive = true;
}

