
#include <libclientserver.h>


/**
 * SplitOne
 * @param[in] str The source string
 * @param[out] left This will be the value left of the deliminator
 * @param[out] right This will be the value right of the deliminator
 * @param[in] delim the value to split on
 * @return will return true if the operation succeeds.
 *
 * This function can fail if the deliminator is not int he string.
 * This function will also remvoe the deliminator from the output strings.
 *
 */
bool String::SplitOne(const std::string *str, std::string *left, std::string *right, const std::string delim)
{
	size_t position = str->find_first_of(delim);

	if (position == std::string::npos)
		return false;

	*left = str->substr(0, position);
	*right = str->substr(position + 1, str->length() - position - 1);

	return true;
}


