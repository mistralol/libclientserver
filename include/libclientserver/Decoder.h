
/**
 * @class Decoder
 *
 * The static Decoder class for converting various data types from a string back to their original after being read from a socket
 *
 */

class Decoder
{
	public:
		static bool Hex2Str(const std::string &str, std::string &out);
		static bool Str2Buf(const std::string &str, char **buf, size_t *buflen);

		static bool Str2Char(const std::string &str, char value);
		static bool Str2UChar(const std::string &str, unsigned char value);
		static bool Str2Short(const std::string &str, short value);
		static bool Str2UShort(const std::string &str, unsigned short value);
		static bool Str2Int(const std::string &str, int value);
		static bool Str2UInt(const std::string &str, unsigned int value);
		static bool Str2Long(const std::string &str, long value);
		static bool Str2ULong(const std::string &str, unsigned long value);
		static bool Str2Float(const std::string &str, float value);
		static bool Str2Double(const std::string &str, double value);
};


