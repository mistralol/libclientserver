
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
 * If the function fails or cannot find any occurances of the delimineter then an empty list
 * will be returned
 *
 */
bool String::Split(const std::string *str, const std::string delim, std::list<std::string> *lst)
{
	size_t position = str->find_first_of(delim);
	lst->clear();

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
 * @param[in] delim1 the outer value to split on
 * @param[in] delim2 the inner value to split on
 * @param[out] lst the output list if items found
 * @return will return true if the operation succeeds.
 *
 * This function can fail if the deliminator is not in the outer string.
 * This function will also fail if the deliminator is not in the inner string.
 * This functions will also fail if it produced a duplucate map value;
 *
 * Example: Split("HELLO=WORLD IT=WORKS", " ", "=", map);
 *
 */
bool String::Split(const std::string *str, const std::string delim1, const std::string delim2, std::map<std::string, std::string> *map)
{
	std::list<std::string> lst;
	map->clear();

	if (Split(str, delim1, &lst) == false)
		return false;

	std::list<std::string>::iterator it = lst.begin();
	while(it != lst.end())
	{
		std::string innerstr = *it;
		std::string left;
		std::string right;

		if (SplitOne(&innerstr, &left, &right, delim2) == false)
			return false;

		//Find a duplicate
		if (map->find(left) != map->end())
			return false;

		map->insert( std::pair<std::string, std::string>(left, right));
		it++;
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
 * If the function fails or cannot find any occurances of the delimineter then an empty list
 * will be returned
 */
bool String::Split(const std::string *str, const std::string delim, std::vector<std::string> *lst)
{
	size_t position = str->find_first_of(delim);
	lst->clear();

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
 * Join
 * @param[in] vec A vector of string
 * @param[in] delim the value to join with
 * @return the output string
 *
 * This function will join string together seperated by the delim paramater.
 */
std::string String::Join(const std::vector<std::string> *vec, const std::string &delim)
{
	std::vector<std::string>::const_iterator it = vec->begin();
	std::string str = "";
	while(it != vec->end())
	{
		str += *it;
		it++;
		if (it != vec->end())
			str += delim;
	}
	return str;
}

/**
 * Join
 * @param[in] lst A list of strings
 * @param[in] delim the value to join with
 * @param[out] str the output string
 *
 * This function will join string together seperated by the delim paramater.
 */
std::string String::Join(const std::list<std::string> *lst, const std::string &delim)
{
	std::list<std::string>::const_iterator it = lst->begin();
	std::string str = "";
	while(it != lst->end())
	{
		str += *it;
		it++;
		if (it != lst->end())
			str += delim;
	}
	return str;
}


