
#include <libclientserver.h>

void ClientBase::Init()
{
	memset(&m_SoftTimeout, 0, sizeof(m_SoftTimeout));
	memset(&m_HardTimeout, 0, sizeof(m_HardTimeout));
	memset(&m_ReConnectTimeout, 0, sizeof(m_ReConnectTimeout));
	
	m_SoftTimeout.tv_sec = 5;
	m_HardTimeout.tv_sec = 30;
	m_ReConnectTimeout.tv_sec = 1;

	m_LastID = 1;

	m_Handler = NULL;
}

void ClientBase::WaitForConnect()
{
	struct timespec ts;
	ts.tv_sec = 30;
	ts.tv_nsec = 0;
	while(WaitForConnect(&ts) == false) { }
}

bool ClientBase::WaitForConnect(const struct timespec *Timeout)
{
	ScopedLock lock(&m_ConnectedMutex);

	while(IsConnected() == false)
	{
		if (m_ConnectedMutex.Wait(Timeout) == 0)
		{
			if (IsConnected())
				return true;
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
	struct RequestMapEntry Entry;

	memset(&Entry, 0, sizeof(Entry));
	Entry.id = m_LastID++;
	Entry.Response = response;
	Entry.ValidResponse = false;
	Entry.KeepAlive = false;
	
	request->SetID(Entry.id);
	
	m_rmap.Add(&Entry);
	if (DoSendRequest(request, SoftTimeout) == false)	//Get Out quickly option - happens when we are disconnected
	{
		m_rmap.Remove(&Entry);
		return false;
	}
	m_rmap.Wait(&Entry, SoftTimeout, HardTimeout);
	m_rmap.Remove(&Entry);

	return Entry.ValidResponse;
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

void ClientBase::RaiseOnSendError(int err, const std::string &str)
{
	if (m_Handler != NULL)
		m_Handler->OnSendError(err, str);
}

void ClientBase::RaiseOnDisconnect(int err, const std::string &str)
{
	if (m_Handler != NULL)
		m_Handler->OnDisconnect(err, str);
}

void ClientBase::RaiseOnResponse(Request *response)
{
	if (m_Handler != NULL)
	{
		if (m_Handler->OnResponse(response) == false)
		{
			return;
		}
	}
	m_rmap.WakeUp(response);
}

void ClientBase::RaiseOnKeepAlive(Request *response)
{
	if (m_Handler != NULL)
	{
		if (m_Handler->OnKeepAlive(response) == false)
		{
			return;
		}
	}
	m_rmap.KeepAlive(response);
}

void ClientBase::RaiseOnEvent(Request *event)
{
	if (m_Handler != NULL)
	{
		if (m_Handler->OnEvent(event) == false)
		{
			return;
		}
	}
}

void ClientBase::RaiseOnData(const std::string *str)
{
	std::string command = "";
	std::string args = "";

	if (String::SplitOne(str, &command, &args, " ") == false)
	{
		RaiseOnBadLine(str);
		return;
	}

	if (command == "RESPONSE")
	{
		Request response;
		if (response.Decode(&args) == false)
		{
			RaiseOnBadLine(str);
			return;
		}
		printf("Response: %s\n", str->c_str());
		RaiseOnResponse(&response);
		return;
	}

	if (command == "KEEPALIVE")
	{
		Request keepalive;
		if (keepalive.Decode(&args) == false)
		{
			RaiseOnBadLine(str);
			return;
		}
		printf("KeepAlive: %s", str->c_str());
		RaiseOnKeepAlive(&keepalive);
		return;
	}

	if (command == "EVENT")
	{
		Request event;
		if (event.Decode(&args) == false)
		{
			RaiseOnBadLine(str);
			return;
		}
		printf("Event: %s", str->c_str());
		RaiseOnEvent(&event);
		return;
	}

	RaiseOnBadLine(str);
	return;
}

void ClientBase::RaiseOnBadLine(const std::string *str)
{
	if (m_Handler != NULL)
		m_Handler->OnBadLine(str);

	printf("BadLine: %s", str->c_str());
}

