
#include <libclientserver.h>

/**
 * Sanity
 * @param[in] str The source string
 * @return will return true if it passes the sanity test
 *
 * This function will return false if str contains certain chars
 * Currently the str must have chars in the following sets
 *
 * const std::string chk1 = "abcdefghijklmnopqrstuvwxyz";
 * const std::string chk2 = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
 * const std::string chk3 = "0123456789";
 * const std::string chk4 = " -";
 */
bool String::Sanity(const std::string *str)
{
	const std::string chk1 = "abcdefghijklmnopqrstuvwxyz";
	const std::string chk2 = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	const std::string chk3 = "0123456789";
	const std::string chk4 = " -";
	const std::string chk = chk1 + chk2 + chk3 + chk4;

	if (Exists(str, &chk) == false)
		return false;
	return true;
}

/**
 * Sanity
 * @param[in] str The source string
 * @param[in] extra Extra set of chars to compare with
 * @return will return true if it passes the sanity test
 *
 * Does the same as Sanity but allows some extra chars to pass the test
 *
 */
bool String::Sanity(const std::string *str, const std::string *extra)
{
	const std::string chk1 = "abcdefghijklmnopqrstuvwxyz";
	const std::string chk2 = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	const std::string chk3 = "0123456789";
	const std::string chk4 = " -";
	const std::string chk = chk1 + chk2 + chk3 + chk4 + *extra;

	if (Exists(str, &chk) == false)
		return false;
	return true;
}


static int StringSortCompare(const std::string &s1, const std::string &s2)
{
	size_t pos1 = 0;
	size_t pos2 = 0;
	while(1)
	{
		while(s1[pos1] != '\0' && s1[pos2] != '\0')
		{
			if (isdigit(s1[pos1]))
				break;
			if (isdigit(s2[pos2]))
				break;

			if (tolower(s1[pos1]) != tolower(s2[pos2]))
				return tolower(s1[pos1]) - tolower(s2[pos2]);

			pos1++;
			pos2++;
		}

		if (s1[pos1] == '\0' && s2[pos2] == '\0')
			return 0;

		if (s1[pos1] == '\0')
			return -1;

		if (s2[pos2] == '\0')
			return 1;

		int i1 = atoi(&s1[pos2]);
		int i2 = atoi(&s2[pos2]);

		if (i1 > i2)
			return 1;
		if (i1 < i2)
			return -1;

		//Advance over the number we used on both strings
		while(s1[pos1] != '\0' && isdigit(s1[pos1]) == true)
			pos1++;
		while(s2[pos2] != '\0' && isdigit(s2[pos2]) == true)
			pos2++;
	}
	abort(); //Should not be reached
	return 0;
}

/**
 * NSort
 * @param[in] lst of strings
 * @param[out] Output list of strings after sorting
 * @return     Nothing the function never fails
 *
 * This function will sort the data in std::list<std:string> in natural human order
 * this will mean that it will make items like item0001 appear before item1000
 */
void String::NSort(const std::list<std::string> &lst, std::list<std::string> &ret)
{
	ret.clear();
	for(std::list<std::string>::const_iterator it = lst.begin(); it != lst.end(); it++)
	{
		bool added = false;
		for(std::list<std::string>::iterator it2 = ret.begin(); it2 != ret.end(); it2++)
		{
			int cmp = StringSortCompare(*it, *it2);
			if (cmp <= 0)
			{
				ret.insert(it2, *it);
				added = true;
				break;
			}
		}
		if (added == false)
			ret.push_back(*it);
	}
}

/**
 * Exists
 * @param[in] str1 The source string
 * @param[in] str2 The string to compare with
 * @return will return true if all the char's in str1 exist in str2
 *
 * This function will return false if str1 contains a char not in str2
 *
 */
bool String::Exists(const std::string *str1, const std::string *str2)
{
	const char *s1 = str1->c_str();

	while(*s1 != 0)
	{
		const char *s2 = str2->c_str();
		bool found = false;
		while(*s2 != 0)
		{
			if (*s1 == *s2)
			{
				found = true;
				break;
			}
			s2++;
		}
		if (found == false)
			return false;
		s1++;
	}
	return true;
}

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
	{
		lst.push_back(*str); //We may only have a single item
	}

	std::list<std::string>::iterator it = lst.begin();
	while(it != lst.end())
	{
		std::string innerstr = *it;
		std::string left;
		std::string right;

		if (SplitOne(&innerstr, &left, &right, delim2) == false)
		{
			return false;
		}

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
 * @return The output string
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

/**
 * Join
 * @param[in] lst A list of strings
 * @param[in] delim1 the value to join with
 * @param[in] delim2 the value to join with
 * @return The output string
 *
 * This function will join together a map<std::string, std::string> to the format of Key<delim2>Pair<delim1>Key<delim2>Pair
 */
std::string String::Join(const std::map<std::string, std::string> *map, const std::string &delim1, const std::string &delim2)
{
	std::map<std::string, std::string>::const_iterator it = map->begin();
	std::string str = "";

	while(it != map->end())
	{
		str += it->first;
		str += delim2;
		str += it->second;
		it++;
		if (it != map->end())
			str += delim1;
	}

	return str;
}

/**
 * Chomp
 * @param[in] str a pointer to a string
 * @param[in] value the char to remove from the end of the string
 * @return The resulting string
 *
 * This function will remove the last trailing char from the end of the string if it exists
 * It is typically used to remove a newline etc.. from input strings.
 */
std::string String::Chomp(const std::string *str, char value)
{
	if (str->at(str->length() - 1) == value)
		return str->substr(0, str->length() - 1);
	return *str;
}

/**
 * Chomp
 * @param[in] str a pointer to a string
 * @param[in] values the char to remove from the end of the string
 * @return The resulting string
 *
 * This function will remove the last trailing char from the end of the string if it exists
 * It is typically used to remove a newline etc.. from input strings.
 */
std::string String::Chomp(const std::string *str, const std::string values)
{
	std::string copy = *str;
restart:
	for(size_t i =0;i<values.length();i++)
	{
		if (copy[copy.length() - 1] == values[i])
		{
			copy = str->substr(0, copy.length() - 1);
			goto restart;
		}
	}
	return copy;
}

/**
 * Check
 * @param[in] str a pointer to a string to check
 * @param[in] values A list of charaters to check for
 * return True if any of the character in values exist in str
 *
 * Used to check if any of the chars in values exist in str
 */
bool String::Check(const std::string *str, const std::string values)
{
	for(size_t i=0;i<values.length();i++)
	{
		if (str->find(values[i]) != std::string::npos)
			return true;
	}
	return false;
}

/**
 * Random
 * @param[in] charset The avilable characters to use to generate the random string
 * @param[in] length The length of the output string that is required
 * @return The output string
 *
 * This function will generate a random string from specific char's for a specific length of string
 */
std::string String::Random(const std::string charset, int length)
{
	unsigned int seed = time(NULL);
	std::string str = "";
	int clen = charset.length();

	for(int i = 0;i<length;i++) {
		int ran = rand_r(&seed) % clen;
		str += charset[ran];
	}
	return str;
}

/**
 * Random
 * @param[in] length The length of the output string that is required
 * @return The output string
 *
 * This function will generate a random string for a specific length of string using charaters a-Z and 0-9
 *
 */
std::string String::Random(int length)
{
	return Random("abcdefghijklmnopquvwxyzABCDEFGHIJKLMNOPQWXYZ012345789", length);
}


