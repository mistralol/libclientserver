
class FileUtil
{
	public:
		static bool Exists(const std::string &fname);
		static int Read(const std::string &fname, std::string *buffer);
		static int Write(const std::string &fname, const std::string &buffer);
		static int WriteSync(const std::string &fname, const std::string &buffer);
		static int Sync(const std::string &fname);
};

