
/** 
 * @class String
 * @brief Utility / Helper Functions for strings
 *
 * Provides various utility functions for helping to deal with string operations
 *
 */

class String
{
	public:
		static bool StartsWith(const std::string &str, const std::string &needle);
		static bool Contains(const std::string &str, const std::string &needle);

		static std::string ToLower(const std::string &str);
		static std::string ToUpper(const std::string &str);
	
		static bool Sanity(const std::string &str);
		static bool Sanity(const std::string &str, const std::string &extra);

		static void NSort(const std::list<std::string> &, std::list<std::string> &);

		static bool Exists(const std::string &str1, const std::string &str2);

		static bool SplitOne(const std::string *str, std::string *left, std::string *right, const std::string delim);

		static bool Split(const std::string *str, const std::string delim, std::list<std::string> *lst);
		static bool Split(const std::string *str, const std::string delim, std::vector<std::string> *lst);

		static bool Split(const std::string *str, const std::string delim1, const std::string delim2, std::map<std::string, std::string> *map);

		static std::string Join(const std::vector<std::string> *vec, const std::string &delim);
		static std::string Join(const std::list<std::string> *vec, const std::string &delim);
		static std::string Join(const std::map<std::string, std::string> *map, const std::string &delim1, const std::string &delim2);

		static std::string Chomp(const std::string *str, char value);
		static std::string Chomp(const std::string *str, const std::string values);

		static bool Check(const std::string *str, const std::string values);

		static std::string Random(const std::string charset, int length);
		static std::string Random(int length);
		
		static bool ToInt(const std::string *str, int *value);
		static bool ToLong(const std::string *str, long *value);
};


