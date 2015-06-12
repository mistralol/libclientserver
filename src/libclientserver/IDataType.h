
/**
 * @class IDataType
 * @brief Interface for encoding / decoding a class
 *
 * Interface for encoding / decoding a class
 */

class IDataType
{
public:
	std::string Encode() = 0;
	void Decode() = 0;

};


