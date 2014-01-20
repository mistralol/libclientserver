
/** 
 * @class String
 * @brief Utility / Helper Functions for strings
 * Provides various utility functions for helping to deal with string operations
 */

class String
{
	public:
		static bool SplitOne(const std::string *str, std::string *left, std::string *right, const std::string delim);

		static bool Split(const std::string *str, const std::string delim, std::list<std::string> *lst);
		static bool Split(const std::string *str, const std::string delim, std::vector<std::string> *lst);

		static bool Split(const std::string *str, const std::string delim1, const std::string delim2, std::map<std::string, std::string> *map);

		static std::string Join(const std::vector<std::string> *vec, const std::string &delim);
		static std::string Join(const std::list<std::string> *vec, const std::string &delim);

};


