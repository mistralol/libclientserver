
#include <libclientserver.h>

Uuid::Uuid()
{
	uuid_generate(m_value);
}

Uuid::Uuid(const std::string str)
{
	if (uuid_parse(str.c_str(), m_value) < 0)
		throw(std::runtime_error("Cannot parse Uuid"));
}

Uuid::~Uuid()
{
	uuid_clear(m_value);
}
		
std::string Uuid::ToString()
{
	char buf[37];
	uuid_unparse_upper(m_value, buf);
	return std::string(buf);
}

Uuid *Uuid::TryParse(const std::string str)
{
	try {
		return new Uuid(str);
	}
	catch(...)
	{
		return NULL;
	}
}

