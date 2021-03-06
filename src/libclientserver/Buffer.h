
/** 
 * @class Buffer
 * @brief Provides a buffer class for working with file descriptors in non-blocking more
 *
 * An easy to use class that makes non-blocking io more painless to use.
 * Make sure you call Init after creating an instance of this class.
 *
 */

class Buffer
{
	public:
		Buffer();
		~Buffer();

		int Init();
		int Init(size_t buflen);

		int Read(int fd);
		int Write(int fd);
		int WriteBuffered(int fd, const char *buf, size_t length);
		int PushData(const char *buf, size_t length);
		int PullData(char *buf, size_t length);
		void Clear();

		bool GetLine(std::string *str);
		bool GetLine(std::string *str, const char terminator);
		bool GetLine(std::string *str, const std::string &ending);
		bool GetLine(std::string *str, const char *ending);

		void Shift(size_t size);

		void *GetPtr();

		size_t GetDataLength();
		size_t GetFreeSpace();
		size_t GetTotalSpace();

		void SetChunkSize(size_t size);
		size_t GetChunkSize();

		int SetMaxSize(size_t size);
		size_t GetMaxSize();

		bool Shrink();

	private:
		bool ReSize(size_t newsize);

	private:
		char *m_buffer;
		size_t m_buffer_length;
		size_t m_buffer_used;
		size_t m_chunk_size;
		size_t m_max_size;
		

};

