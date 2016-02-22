
#include <libclientserver.h>

ClientUnixSelected::ClientUnixSelected(const std::string path)
{
	Init();
	m_path = path;
	m_selector = new Selector();
	m_connection = NULL;
}

ClientUnixSelected::~ClientUnixSelected()
{
	if (IsConnected())
		Disconnect();
	delete m_selector;

}

void ClientUnixSelected::Connect()
{
	ScopedLock lock = ScopedLock(&m_mutex);
	if (m_connection)
		abort();
	m_connection = new ClientUnixSelectedConnection(this, m_path);
	m_selector->Add(m_connection);
}

bool ClientUnixSelected::IsConnected()
{
	ScopedLock lock = ScopedLock(&m_mutex);
	if (m_connection == NULL)
		return false;
	return m_connection->IsConnected();
}

void ClientUnixSelected::Disconnect()
{
	ScopedLock lock = ScopedLock(&m_mutex);
	if (m_connection == NULL)
		return;
	m_selector->Remove(m_connection);
	delete m_connection;
	m_connection = NULL;
}

bool ClientUnixSelected::SendLine(const std::string *str, const struct timespec *Timeout)
{
	ScopedLock lock = ScopedLock(&m_mutex);
	if (m_connection == NULL)
		return false;
	return m_connection->SendLine(m_selector, str);
}

//This does not need a lock because it is used only by the ClientUnixSelectedConnection to get the delay
//This function may be moved to the base class.
void ClientUnixSelected::GetReconnectDelay(struct timespec *tv)
{
	tv->tv_sec = m_ReConnectDelay.tv_sec;
	tv->tv_nsec = m_ReConnectDelay.tv_nsec;
}

