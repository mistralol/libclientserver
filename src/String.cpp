
#include <libclientserver.h>


/**
 * SplitOne
 * @param[in] str The source string
 * @param[out] left This will be the value left of the deliminator
 * @param[out] right This will be the value right of the deliminator
 * @param[in] delim the value to split on
 * @return will return true if the operation succeeds.
 *
 * This function can fail if the deliminator is not in the string.
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

/**
 * Split
 * @param[in] str The source string
 * @param[in] delim the value to split on
 * @param[out] lst the output list if items found
 * @return will return true if the operation succeeds.
 *
 * This function can fail if the deliminator is not in the string.
 * This function will also remvoe the deliminator from the output strings.
 *
 */
bool String::Split(const std::string *str, const std::string delim, std::list<std::string> *lst)
{
	size_t position = str->find_first_of(delim);

	if (position == std::string::npos)
		return false;

	std::string left = str->substr(0, position);
	std::string right = str->substr(position + 1, str->length() - position - 1);

	while(true)
	{
		lst->push_back(left);

		size_t position = right.find_first_of(delim);
		if (position == std::string::npos)
		{
			lst->push_back(right);
			return true;
		}

		left = right.substr(0, position);
		right = right.substr(position + 1, right.length() - position - 1);
	}

	return true;

}

/**
 * Split
 * @param[in] str The source string
 * @param[in] delim the value to split on
 * @param[out] lst the output list if items found
 * @return will return true if the operation succeeds.
 *
 * This function can fail if the deliminator is not in the string.
 * This function will also remvoe the deliminator from the output strings.
 *
 */
bool String::Split(const std::string *str, const std::string delim, std::vector<std::string> *lst)
{
	size_t position = str->find_first_of(delim);

	if (position == std::string::npos)
		return false;

	std::string left = str->substr(0, position);
	std::string right = str->substr(position + 1, str->length() - position - 1);

	while(true)
	{
		lst->push_back(left);

		size_t position = right.find_first_of(delim);
		if (position == std::string::npos)
		{
			lst->push_back(right);
			return true;
		}

		left = right.substr(0, position);
		right = right.substr(position + 1, right.length() - position - 1);
	}

	return true;
}

