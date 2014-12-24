
#include <libclientserver.h>

Request::Request()
{
	m_id = -1;
	m_command = "";
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

std::list<std::string> Request::GetArgList()
{
	std::list<std::string> lst;
	for(std::map<std::string, std::string>::iterator it = m_args.begin(); it != m_args.end(); it++)
	{
		lst.push_back(it->first);
	}
	return lst;
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

std::string Request::GetArg(const std::string Key)
{
	std::map<std::string, std::string>::iterator it = m_args.find(Key);
#ifdef DEBUG
	if (it == m_args.end())
		abort();
#endif
	return it->second;
}

bool Request::GetInt(const std::string Key, int *value)
{
	if (HasArg(Key) == false)
		return false;
	std::string str = GetArg(Key);
	*value = atoi(str.c_str());
	return true; //FIXME: Improve error handling
}

void Request::SetArg(const std::string *Key, const std::string *Value)
{
	m_args[*Key] = *Value;
}

void Request::SetArg(const std::string Key, const std::string Value)
{
	m_args[Key] = Value;
}

void Request::SetArg(const std::string Key, const std::string *Value)
{
	m_args[Key] = *Value;
}

void Request::SetArg(const std::string Key, int Value)
{
	char *buf = NULL;
	asprintf(&buf, "%d", Value);
	m_args[Key] = buf;
	free(buf);
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
	std::stringstream ss;

	ss << m_id << " " << m_command;

	for(std::map<std::string, std::string>::iterator it = m_args.begin(); it != m_args.end(); it++) {
		ss << " " << it->first << "=" << Encoder::ToStr(it->second);
	}
	return ss.str();
}

bool Request::Decode(const std::string *str)
{
	std::string sid = "";
	std::string args = "";
	std::string right = "";

	if (String::SplitOne(str, &sid, &right, " ") == false)
		return false;

	errno = 0;
	m_id = strtoull(sid.c_str(), NULL, 10);
	if (errno != 0)
	{
		return false;
	}

	if (String::SplitOne(&right, &m_command, &args, " ") == false)
	{
		m_command = right;
		return true; //No Args
	}

	if (args.size() > 0)
	{
		if (String::Split(&args, " ", "=", &m_args) == false)
		{
			return false;
		}

		for(std::map<std::string, std::string>::iterator it = m_args.begin(); it != m_args.end(); it++)
		{
			std::string tmp;
			Decoder::ToStr(it->second, tmp);
			it->second = tmp;
		}
	}

	return true;
}

