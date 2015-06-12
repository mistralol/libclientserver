
/**
 * @class Decoder
 *
 * The static Decoder class for converting various data types from a string back to their original after being read from a socket
 *
 */

class Decoder
{
	public:
		static bool ToStr(const std::string &str, std::string &out);

		static bool ToLong(const std::string &str, long *value);
		static bool ToInt(const std::string &str, int *value);
		static bool ToShort(const std::string &str, short *value);
		static bool ToChar(const std::string &str, char *value);
		static bool ToFloat(const std::string &str, float *value);
		static bool ToDouble(const std::string &str, double *value);

		static bool ToULong(const std::string &str, unsigned long *value);
		static bool ToUInt(const std::string &str, unsigned int *value);
		static bool ToUShort(const std::string &str, unsigned short *value);
		static bool ToUChar(const std::string &str, unsigned char *value);

		static bool ToTimeSpec(const std::string &str, struct timespec *ts);
		static bool ToTimeVal(const std::string &str, struct timeval *ts);

};


