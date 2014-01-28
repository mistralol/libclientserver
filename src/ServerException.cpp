
#include <libclientserver.h>

ServerException::ServerException()
{

}

ServerException::ServerException(const std::string Message)
{
	m_msg = Message;
}

ServerException::ServerException(const int err)
{
	m_msg = Errno::ToStr(err);
}

ServerException::~ServerException() throw()
{

}

const char *ServerException::what() const throw()
{
	return m_msg.c_str();
}


