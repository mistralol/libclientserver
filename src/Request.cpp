
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

bool Request::GetInt(const std::string &Key, int *value)
{
	if (HasArg(Key) == false)
		return false;
	std::string str = GetArg(Key);
	*value = atoi(str.c_str());
	return true; //FIXME: Improve error handling
}

bool Request::GetUInt(const std::string &Key, unsigned int *Value)
{
	if (HasArg(Key) == false)
		return false;
	std::string str = GetArg(Key);
	*Value = atoi(str.c_str());
	return true; //FIXME: Improve error handling
}

bool Request::GetBool(const std::string &Key, bool *Value)
{
	int tmp = 0;
	if (HasArg(Key) == false)
		return false;
	std::string str = GetArg(Key);
	tmp = atoi(str.c_str());
	if (tmp)
	{
		*Value = true;
	}
	else
	{
		*Value = false;
	}
	return true; //FIXME: Improve error handling
}

bool Request::GetString(const std::string &Key, std::string *str)
{
	if (HasArg(Key) == false)
		return false;
	*str = GetArg(Key);
	return true;
}

bool Request::GetListString(const std::string &Key, std::list<std::string> *lst)
{
	if (HasArg(Key) == false)
		return false;
	std::string str = GetArg(Key);
	std::list<std::string> src;
	lst->clear();
	if (String::Split(&str, ",", &src) == false)
	{
		//Treat it as a list with a single item
		std::string tmp;
		if (Decoder::ToStr(str, tmp) == false)
			return false;
		lst->push_back(tmp);
		return true;
	}
	std::list<std::string>::iterator it = src.begin();
	while(it != src.end())
	{
		std::string tmp = "";
		if (Decoder::ToStr(*it, tmp) == false)
			return false;
		lst->push_back(tmp);
		it++;
	}
	return true;
}

bool Request::GetVectorString(const std::string &Key, std::vector<std::string> *vec)
{
	std::list<std::string> lst;
	if (GetListString(Key, &lst) == false)
		return false;
	vec->clear();
	while(lst.size() > 0)
	{
		std::string tmp = lst.front();
		lst.pop_front();
		vec->push_back(tmp);
	}
	return true;
}

bool Request::GetTimeSpec(const std::string &Key, struct timespec *ts)
{
	if (GetInt(Key + ".tv_sec", (int *) &ts->tv_sec) == false)
		return false;
	if (GetInt(Key + ".tv_nsec", (int *) &ts->tv_nsec) == false)
		return false;
	return true;
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
	if (asprintf(&buf, "%d", Value) < 0)
		abort();
	m_args[Key] = buf;
	free(buf);
}

void Request::SetArg(const std::string Key, unsigned int Value)
{
	char *buf = NULL;
	if (asprintf(&buf, "%u", Value) < 0)
		abort();
	m_args[Key] = buf;
	free(buf);
}

void Request::SetArg(const std::string Key, bool Value)
{
	if (Value)
	{
		SetArg(Key, 1);
	}
	else
	{
		SetArg(Key, 0);
	}
}

void Request::SetArg(const std::string Key, std::list<std::string> *lst)
{
	std::stringstream ss;
	std::list<std::string>::iterator it = lst->begin();
	while(it != lst->end())
	{
		ss << Encoder::ToStr(*it);
		it++;
		if (it != lst->end())
			ss << ",";
	}
	SetArg(Key, ss.str());
}

void Request::SetArg(const std::string Key, std::vector<std::string> *lst)
{
	std::stringstream ss;
	std::vector<std::string>::iterator it = lst->begin();
	while(it != lst->end())
	{
		ss << Encoder::ToStr(*it);
		it++;
		if (it != lst->end())
			ss << ",";
	}
	SetArg(Key, ss.str());
}

void Request::SetArg(const std::string Key, const timespec *ts)
{
	//FIXME: Remove Casts also fix the get
	SetArg(Key, "0"); //So that HasArg works
	SetArg(Key + ".tv_sec", (int) ts->tv_sec);
	SetArg(Key + ".tv_nsec", (int) ts->tv_nsec);
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
			if (Decoder::ToStr(it->second, tmp) == false)
				return false;
			it->second = tmp;
		}
	}

	return true;
}

