
/**
 * @class Encoder
 *
 * The static Encoder class for converting various data types to a string suitable to send over a socket
 *
 */

class Encoder
{
	public:
		static std::string ToStr(const std::string &str);

		static std::string ToStr(const long value);
		static std::string ToStr(const int value);
		static std::string ToStr(const short value);
		static std::string ToStr(const char value);
		static std::string ToStr(const float value);
		static std::string ToStr(const double value);

		static std::string ToStr(const unsigned long value);
		static std::string ToStr(const unsigned int value);
		static std::string ToStr(const unsigned short value);
		static std::string ToStr(const unsigned char value);

		static std::string ToStr(const struct timespec *ts);
		static std::string ToStr(const struct timeval *ts);

};


