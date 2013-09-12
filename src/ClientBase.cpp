
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
	//FIXME: Should be try lock
/*
	if (m_ConnectedMutex.lock() == false)
		return false;
*/
	m_ConnectedMutex.lock();

	if (IsConnected() == true)
	{
		m_ConnectedMutex.unlock();
		return true;
	}

	//m_ConnectedCond.timed_wait(m_ConnectedMutex);

	m_ConnectedMutex.unlock();
	abort();

	m_ConnectedCond.notify_all();
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
	abort();
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



