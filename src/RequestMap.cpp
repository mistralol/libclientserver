

#include <libclientserver.h>

RequestMap::RequestMap()
{

}

RequestMap::~RequestMap()
{

}

void RequestMap::Add(RequestMapEntry *Entry)
{
	ScopedLock lock(&m_Mutex);
	int id = Entry->id;
	std::map<int, RequestMapEntry *>::iterator it = m_map.find(id);
	if (it != m_map.end())
		abort();	//Bug! Added duplicate

	m_map[id] = Entry;
}

void RequestMap::Remove(RequestMapEntry *Entry)
{
	ScopedLock lock(&m_Mutex);
	int id = Entry->id;
	std::map<int, RequestMapEntry *>::iterator it = m_map.find(id);
	if (it == m_map.end())
		abort();	//Bug! Attempted to remove something that does not exist

	m_map.erase(it);
}

bool RequestMap::Exists(RequestMapEntry *Entry)
{
	ScopedLock lock(&m_Mutex);
	int id = Entry->id;
	std::map<int, RequestMapEntry *>::iterator it = m_map.find(id);
	if (it == m_map.end())
		return false;
	return true;
}

bool RequestMap::IsComplete(RequestMapEntry *Entry)
{
	ScopedLock lock(&m_Mutex);
	int id = Entry->id;
	std::map<int, RequestMapEntry *>::iterator it = m_map.find(id);
	if (it == m_map.end())
		abort(); //Api mis-use

	if (Entry->ValidResponse)
		return true;
	return false;
}

bool RequestMap::Wait(RequestMapEntry *Entry, const struct timespec *SoftTimeout, const struct timespec *HardTimeout)
{
	ScopedLock lock(&m_Mutex);
	int id = Entry->id;
	std::map<int, RequestMapEntry *>::iterator it = m_map.find(id);
	if (it == m_map.end())
		return false; //Trying to wait on something that does not exist

	//FIXME: Current this is only precise to 1 second. It should be better!
	struct timespec CurrentTime;
	struct timespec DeadLine;
	struct timespec HardLine;
	struct timespec rem; //Remaiing time

	Time::MonoTonic(&CurrentTime);

	Time::Add(&CurrentTime, SoftTimeout, &DeadLine);
	Time::Add(&CurrentTime, HardTimeout, &HardLine);

	while(Entry->ValidResponse == false)
	{
		Time::MonoTonic(&CurrentTime);

		//If we cross a dead line give up
		if (Time::IsGreater(&CurrentTime, &DeadLine))
			return false;

		//If we cross a hard line give up
		if (Time::IsGreater(&CurrentTime, &HardLine))
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

bool RequestMap::WakeUp(Json::Value &response)
{
	int id = 0;
	if (response.isMember("_ID") && response["_ID"].isInt())
	{
		id = response["_ID"].asInt();
	}
	else
	{
		return false;
	}
	ScopedLock lock(&m_Mutex);
	std::map<int, RequestMapEntry *>::iterator it = m_map.find(id);
	if (it == m_map.end())
		return false; //Nothing left to wakeup. Probably a timeout and has been removed

	*it->second->Response = response;
	it->second->ValidResponse = true;
	m_Mutex.WakeUpAll();
	return true;
}

bool RequestMap::KeepAlive(Json::Value &response)
{
	int id = 0;
	if (response.isMember("_ID") && response["_ID"].isInt())
	{
		id = response["_ID"].asInt();
	}
	else
	{
		return false;
	}
	ScopedLock lock(&m_Mutex);
	std::map<int, RequestMapEntry *>::iterator it = m_map.find(id);
	if (it == m_map.end())
		return false;
	it->second->KeepAlive = true;
	return true;
}

