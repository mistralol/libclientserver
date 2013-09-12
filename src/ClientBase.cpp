
#include <libclientserver.h>

void ClientBase::Init()
{
	memset(&m_SoftTimeout, 0, sizeof(m_SoftTimeout));
	memset(&m_HardTimeout, 0, sizeof(m_HardTimeout));
	
	m_ReConnectTimeout.tv_sec = 1;
	m_SoftTimeout.tv_sec = 5;
	m_HardTimeout.tv_sec = 30;
}

void ClientBase::WaitForConnect()
{
	struct timespec ts;
	ts.tv_sec = 1;
	ts.tv_nsec = 0;
	while(WaitForConnect(&ts) == false) { }
}

bool ClientBase::WaitForConnect(const struct timespec *Timeout)
{
	boost::system_time timeout = Time::CalcTimeout(Timeout);
	boost::mutex::scoped_lock lock(m_ConnectedMutex);

	while(IsConnected() == true)
	{
		if (m_ConnectedCond.timed_wait(lock, timeout) == false)
		{
			return IsConnected();
		}
	}

	return false;
}

void ClientBase::SetReConnectTimeout(const struct timespec *Timeout)
{
	memcpy(&m_ReConnectTimeout, Timeout, sizeof(m_ReConnectTimeout));
}

void ClientBase::SetSoftTimeout(const struct timespec *SoftTimeout)
{
	memcpy(&m_SoftTimeout, SoftTimeout, sizeof(m_SoftTimeout));
}

void ClientBase::SetHardTimeout(const struct timespec *HardTimeout)
{
	memcpy(&m_HardTimeout, HardTimeout, sizeof(m_HardTimeout));
}

void ClientBase::SendRequest(Request *request, Request *response, const struct timespec *SoftTimeout, const struct timespec *HardTimeout)
{
	abort();
	//Add To Request Map
	//Clear KeepAlive Flag
	//Send The Request
	//Wait For Timeout On Request Map
	//if no Request Avilable Look for KeepAlive - restart if still under the timeout
}

void ClientBase::SendRequest(Request *request, Request *response, const struct timespec *SoftTimeout)
{
	SendRequest(request, response, SoftTimeout, &m_HardTimeout);
}

void ClientBase::SendRequest(Request *request, Request *response)
{
	SendRequest(request, response, &m_SoftTimeout, &m_HardTimeout);
}

void ClientBase::SendCommand(Request *command)
{
	abort();
}

void ClientBase::RaiseOnConnect()
{
	boost::mutex::scoped_lock lock(m_ConnectedMutex);
#ifdef DEBUG
	if (IsConnected() == false)
		abort();	//Apparently Not Connected. This is a bug in the dervied class for Raiseing the event when not connected
#endif
	m_ConnectedCond.notify_all();
}

void ClientBase::RaiseOnConnectError(int err, const std::string &str)
{
	abort();
}

void ClientBase::RaiseOnDisconnect(int err, const std::string &str)
{
	abort();
}

void ClientBase::RaiseOnResponse()
{
	abort();
}

void ClientBase::RaiseOnEvent()
{
	abort();
}



