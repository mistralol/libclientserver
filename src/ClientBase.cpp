
#include <libclientserver.h>

/**
 * Init
 *
 * This should be called to intialize any base class member variables and state.
 * 
 */
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

/**
 * WaitForConnect
 *
 * This function provides a method that will sleep until the client reaches a connected state to the server.
 * Please be aware that this function will not return until a connection has been made.
 * However when it returns it is possible the that client could have also been disconnected from the server again.
 */
void ClientBase::WaitForConnect()
{
	struct timespec ts;
	ts.tv_sec = 30;
	ts.tv_nsec = 0;
	while(WaitForConnect(&ts) == false) { }
}

/**
 * WaitForConnect
 * @param[in] Timeout The specified time to wait before returning
 * @return If a connection was made.
 *
 * This function provides a method that will sleep until the client reaches a connected state to the server. Or until the value in timeout has elasped
 * However when it returns it is possible the that client could have also been disconnected from the server again. Event when the function returns true.
 */
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

/**
 * SetReConnectTimeout
 * @param[in] Timeout The time to wait before retrying a connection
 *
 * This function sets the retry time between connections to the server.
 *
 */
void ClientBase::SetReConnectTimeout(const struct timespec *Timeout)
{
	//FIXME: This should be called SetReconnectDelay
	memcpy(&m_ReConnectTimeout, Timeout, sizeof(m_ReConnectTimeout));
}

/**
 * SetSoftTimeout
 * @params[in] Timeout value
 *
 * This function will set the default soft timeout value when sending requests
 * The function will take a copy of the struture that SoftTimeout points to.
 *
 * The soft timeout values can be overridded by the server. If the server sends keepalive messages
 */
void ClientBase::SetSoftTimeout(const struct timespec *SoftTimeout)
{
	memcpy(&m_SoftTimeout, SoftTimeout, sizeof(m_SoftTimeout));
}

/**
 * SetHardTimeout
 * @params[in] Timeout value
 *
 * This function will set the default hard timeout value when sending requests
 * The function will take a copy of the struture that HardTimeout points to.
 *
 * The hard timeout value will never be ignored when sending requests.
 * If it is ignored then it should be considered a bug.
 */
void ClientBase::SetHardTimeout(const struct timespec *HardTimeout)
{
	memcpy(&m_HardTimeout, HardTimeout, sizeof(m_HardTimeout));
}

/**
 * SetHandler
 * @params[in] Handler
 *
 * Register a Handler class which is used by the client so that the client implmenetation can take certain actions when various events occur.
 * These event happen during Disconnect / Connect / Errors
 *
 */
void ClientBase::SetHandler(IClientHandler *Handler)
{
	m_Handler = Handler;
}

/**
 * SendRequest
 * @param[in] request The Client Request
 * @param[out] response The Server Response
 * @param[in] SoftTimeout The soft timeout value
 * @param[in] HardTimeout The hard timeout value
 * @return True when a response is recived from the server
 *
 * This function is the full version of SendRequest with arugments for setting both the Soft/Hard timeout functions.
 * the Soft/Hard timeout values that are specified to this function will override any default that have been set.
 *
 * If the function returns false. The contents of the response will be undefined.
 */
bool ClientBase::SendRequest(Request *request, Request *response, const struct timespec *SoftTimeout, const struct timespec *HardTimeout)
{
	//FIXME: request should be const
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

/**
 * SendRequest
 * @param[in] request The Client Request
 * @param[out] response The Server Response
 * @param[in] SoftTimeout The soft timeout value
 *
 * This function is the partial version of SendRequest with arugments for setting both the Soft timeout functions.
 * the Soft timeout values that are specified to this function will override any default that have been set.
 *
 * the hard timeout value that will be used is the default configured value on the instance of the class.
 *
 * For more details please read the documentation on the full function.
 */
bool ClientBase::SendRequest(Request *request, Request *response, const struct timespec *SoftTimeout)
{
	//FIXME: request should be const
	return SendRequest(request, response, SoftTimeout, &m_HardTimeout);
}

/**
 * SendRequest
 * @param[in] request The Client Request
 * @param[out] response The Server Response
 *
 * This is the basic function for sending a request where the default values of Soft/Hard timeout will be used.
 * For more details please read the documentation on the full function.
 */
bool ClientBase::SendRequest(Request *request, Request *response)
{
	return SendRequest(request, response, &m_SoftTimeout, &m_HardTimeout);
}

/**
 * SendCommand
 * @param[in] command
 * @param[in] Timeout
 * @return True when successful
 *
 * Note this function can return True even when sending the command is not successful. It Only states that the command was queued to be send to the server.
 * If the Client is not Connected to the server then this function will immeditatly return false.
 */
bool ClientBase::SendCommand(Request *command, const struct timespec *Timeout)
{
	//FIXME: command should be const
	return DoSendCommand(command, Timeout);
}

/**
 * SendCommand
 * @param[in] command
 * @param[in] Timeout
 * @return True when successful
 *
 * Note this function can return True even when sending the command is not successful. It Only states that the command was queued to be send to the server.
 * If the Client is not Connected to the server then this function will immeditatly return false.
 */
bool ClientBase::SendCommand(Request *command)
{
	//FIXME: command should be const
	return SendCommand(command, &m_SoftTimeout);
}

/**
 * GetNextID
 * @return The next avilable ID
 *
 * This function is used to generate a unique index for every request that is sent by the client
 */
uint64_t ClientBase::GetNextID()
{
	ScopedLock lock(&m_LastIDMutex);
	m_LastID++;
	return m_LastID;
}

/**
 * RaiseOnConnect
 * 
 * This function is called when a client makes a connection to the server
 */
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

/**
 * RaiseOnConnectError
 * @param[in] err the contents of errno when the error occured
 * @param[in] str a text version of the error
 *
 * The function is called every time a connection fails to the server
 */
void ClientBase::RaiseOnConnectError(int err, const std::string &str)
{
	if (m_Handler != NULL)
		m_Handler->OnConnectError(err, str);
}

/**
 * RaiseOnSendError
 * @param[in] err the contents of errno when the error occured
 * @param[in] str a text version of the error
 *
 * The function is called when there is an error sending data to the server
 */
void ClientBase::RaiseOnSendError(int err, const std::string &str)
{
	if (m_Handler != NULL)
		m_Handler->OnSendError(err, str);
}

/**
 * RaiseOnDisconnect
 * @param[in] err the contents of errno when the error occured
 * @param[in] str a text version of the error
 *
 * The function is called when the client disconnects from the server for any reason.
 * Note that the err value may indicate success which is valid and only suggests that the connection
 * was shutdown cleanly as requested
 */
void ClientBase::RaiseOnDisconnect(int err, const std::string &str)
{
	if (m_Handler != NULL)
		m_Handler->OnDisconnect(err, str);
}

/**
 * RaiseOnResponse
 * @param[in] response
 *
 * This function is called when a response is recived by the client
 */
void ClientBase::RaiseOnResponse(Request *response)
{
	//FIXME: repsonse should be const
	if (m_Handler != NULL)
	{
		if (m_Handler->OnResponse(response) == false)
		{
			return;
		}
	}
	m_rmap.WakeUp(response);
}

/**
 * RaiseOnKeepAlive
 * @param[in] response
 *
 * This function is called when a keepalive is recived by the client
 */
void ClientBase::RaiseOnKeepAlive(Request *response)
{
	//FIXME: repsonse should be const
	if (m_Handler != NULL)
	{
		if (m_Handler->OnKeepAlive(response) == false)
		{
			return;
		}
	}
	m_rmap.KeepAlive(response);
}

/**
 * RaiseOnEvent
 * @param[in] event
 *
 * This function is called when an event is recived by the client
 */
void ClientBase::RaiseOnEvent(Request *event)
{
	//FIXME: event should be const
	if (m_Handler != NULL)
	{
		if (m_Handler->OnEvent(event) == false)
		{
			return;
		}
	}
}

/**
 * RaiseOnData
 * @param[in] str
 *
 * This function is called when a line of data is recived by the client
 */
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

/**
 * RaiseOnBadLine
 * @param[in] str
 *
 * This function is called when a line of bad data / non-parseable data is recived by the client
 */
void ClientBase::RaiseOnBadLine(const std::string *str)
{
	if (m_Handler != NULL)
		m_Handler->OnBadLine(str);

	printf("BadLine: %s", str->c_str());
	//FIXME: We should abort here if the handler doesnt handle
}

