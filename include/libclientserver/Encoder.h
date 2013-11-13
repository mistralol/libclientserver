
/**
 * @class Encoder
 *
 * The static Encoder class for converting various data types to a string suitable to send over a socket
 *
 */

class Encoder
{
	public:
		static std::string Str2Hex(const std::string &str);
		static std::string Buf2Hex(const char *buf, size_t buflen);

		static std::string Char2Str(char value);
		static std::string UChar2Str(unsigned char value);
		static std::string Short2Str(short value);
		static std::string UShort2Str(unsigned short value);
		static std::string Int2Str(int value);
		static std::string UInt2Str(unsigned int value);
		static std::string Long2Str(long value);
		static std::string ULong2Str(unsigned long value);
		static std::string Float2Str(float value);
		static std::string Double2Str(double value);
};


