
#include <libclientserver.h>

#include <boost/lexical_cast.hpp>

Request::Request()
{

}

Request::~Request()
{

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

/*
std::string Request::Encode() {
	std::string str;
	str += m_command;
	str += "\n";
	for(std::map<std::string, std::string>::iterator it = m_args.begin(); it != m_args.end(); it++) {
		str += it->first;
		str += '=';
		str += HexEncode(it->second);
		str += ' ';
	}
	str += "\n";

	return str;
}
*/

