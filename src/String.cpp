
#include <libclientserver.h>

bool String::SplitOne(const std::string *str, std::string *left, std::string *right, const std::string delim)
{
	size_t position = str->find_first_of(delim);

	if (position == std::string::npos)
		return false;

	*left = str->substr(0, position);
	*right = str->substr(position + 1, str->length() - position - 1);

	return true;
}


