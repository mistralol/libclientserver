
#include <libclientserver.h>

#include <boost/lexical_cast.hpp>

Request::Request()
{

}

Request::~Request()
{

}

uint64_t Request::GetID()
{
	return m_id;
}

void Request::SetID(uint64_t ID)
{
	m_id = ID;
}

void Request::SetCommand(const std::string &Command)
{
	m_command = Command;
}

std::string Request::GetCommand()
{
	return m_command;
}

bool Request::HasArg(const std::string &Name)
{
	if (m_args.find(Name) == m_args.end())
		return false;
	return true;
}

std::string Request::GetArg(const std::string *Key)
{
	std::map<std::string, std::string>::iterator it = m_args.find(*Key);
#ifdef DEBUG
	if (it == m_args.end())
		abort();
#endif
	return it->second;
}

void Request::SetArg(const std::string *Key, const std::string *Value)
{
	m_args[*Key] = *Value;
}

void Request::RemoveArg(const std::string *Key)
{
	std::map<std::string, std::string>::iterator it = m_args.find(*Key);
#ifdef DEBUG
	if (it == m_args.end())
		abort();
#endif
	m_args.erase(it);
}

std::string Request::Encode() {
	std::string str;
	str += m_command;
	str += " ";
	str += m_id;
	str += " ";
	for(std::map<std::string, std::string>::iterator it = m_args.begin(); it != m_args.end(); it++) {
		str += it->first;
		str += '=';
		str += HexEncode(it->second);
		str += ' ';
	}
	str += "\n";

	return str;
}

void Request::Decode(const std::string *str)
{
	abort();	
}

