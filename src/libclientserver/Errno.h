
/**
 * @class Errno
 * @brief Utility functions for C++ to deal with errno
 *
 * Utility functions for C++ to deal with errno
 */
class Errno
{
	public:
		static const std::string ToStr();
		static const std::string ToStr(int err);
};


