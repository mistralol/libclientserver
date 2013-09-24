
#include <libclientserver.h>

void ClientBase::Init()
{
	memset(&m_SoftTimeout, 0, sizeof(m_SoftTimeout));
	memset(&m_HardTimeout, 0, sizeof(m_HardTimeout));
	memset(&m_ReConnectTimeout, 0, sizeof(m_ReConnectTimeout));
	
	m_ReConnectTimeout.tv_sec = 1;
	m_SoftTimeout.tv_sec = 5;
	m_HardTimeout.tv_sec = 30;

	m_ReConnectTimeout.tv_sec = 2;
	
	m_Handler = NULL;
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
	ScopedLock lock(&m_ConnectedMutex);

	while(IsConnected() == true)
	{
		if (m_ConnectedMutex.Wait(Timeout) == false)
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

void ClientBase::SetHandler(IClientHandler *Handler)
{
	m_Handler = Handler;
}

bool ClientBase::SendRequest(Request *request, Request *response, const struct timespec *SoftTimeout, const struct timespec *HardTimeout)
{
	abort();
	//Add To Request Map
	//Clear KeepAlive Flag
	//Send The Request
	//Wait For Timeout On Request Map
	//if no Request Avilable Look for KeepAlive - restart if still under the timeout

	//We only use soft timeout here because it has no chance of having a keepalive returned until this function returns
	return DoSendRequest(request, SoftTimeout); 
}

bool ClientBase::SendRequest(Request *request, Request *response, const struct timespec *SoftTimeout)
{
	return SendRequest(request, response, SoftTimeout, &m_HardTimeout);
}

bool ClientBase::SendRequest(Request *request, Request *response)
{
	return SendRequest(request, response, &m_SoftTimeout, &m_HardTimeout);
}

bool ClientBase::SendCommand(Request *command, const struct timespec *Timeout)
{
	return DoSendCommand(command, Timeout);
}

bool ClientBase::SendCommand(Request *command)
{
	return SendCommand(command, &m_SoftTimeout);
}

uint64_t ClientBase::GetNextID()
{
	ScopedLock lock(&m_LastIDMutex);
	m_LastID++;
	return m_LastID;
}

void ClientBase::RaiseOnConnect()
{
	ScopedLock lock(&m_ConnectedMutex);
#ifdef DEBUG
	if (IsConnected() == false)
		abort();	//Apparently Not Connected. This is a bug in the dervied class for Raiseing the event when not connected
#endif
	m_ConnectedMutex.WakeUpAll();
	
	if (m_Handler != NULL)
		m_Handler->OnConnect();
}

void ClientBase::RaiseOnConnectError(int err, const std::string &str)
{
	if (m_Handler != NULL)
		m_Handler->OnConnectError(err, str);
}

void ClientBase::RaiseOnDisconnect(int err, const std::string &str)
{
	if (m_Handler != NULL)
		m_Handler->OnDisconnect(err, str);
}

void ClientBase::RaiseOnResponse()
{
	if (m_Handler != NULL)
		m_Handler->OnResponse();
}

void ClientBase::RaiseOnEvent()
{
	if (m_Handler != NULL)
		m_Handler->OnEvent();
}

void ClientBase::RaiseOnData(const std::string *str)
{
	printf("Unknown Data: %s\n", str->c_str());
	abort();
}


