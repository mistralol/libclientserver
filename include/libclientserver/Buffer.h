class Buffer
{
	public:
		Buffer();
		~Buffer();

		int Init();
		int Init(size_t buflen);

		int Read(int fd);
		int Write(int fd);
		int PushData(const char *buf, size_t length);
		int PullData(char *buf, size_t length);

		bool GetLine(std::string *str);

		void Shift(size_t size);

		void *GetPtr();

		size_t GetDataLength();
		size_t GetFreeSpace();
		size_t GetTotalSpace();

		void SetChunkSize(size_t size);
		size_t GetChunkSize();

		void Shrink();
	private:
		bool ReSize(size_t newsize);

	private:
		char *m_buffer;
		size_t m_buffer_length;
		size_t m_buffer_used;
		size_t m_chunk_size;
		

};

