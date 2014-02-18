
#include <libclientserver.h>

ServerException::ServerException()
{

}

ServerException::ServerException(const int err)
{
	m_msg = Errno::ToStr(err);
}

ServerException::ServerException(const int err, const std::string Message)
{
	m_errno = err;
	m_msg = Message;
}

ServerException::~ServerException() throw()
{

}

const char *ServerException::what() const throw()
{
	return m_msg.c_str();
}


int ServerException::GetErrorNo() const
{
	return m_errno;
}

std::string ServerException::GetErrorMessage() const
{
	return m_msg;
}

