
class ReadBuffer
{
	public:
		ReadBuffer(size_t MaxSize);
		~ReadBuffer();

		char *GetStartPtr();
		size_t GetPosition();

		int Read(int fd);
		bool GetLine(std::string *str);

		void Shift(size_t offset); //Moves data from current position back to 0
		
	private:
		char *m_buffer;
		size_t m_position;
		size_t m_maxsize;

};


