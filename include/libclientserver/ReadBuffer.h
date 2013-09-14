
class ReadBuffer
{
	public:
		ReadBuffer(size_t MaxSize);
		~ReadBuffer();

		char *GetStartPtr();
		size_t GetPosition();

		int Read(int fd);

		void Shift(size_t offset); //Moves data from offset to 0
		


	private:
		char *m_buffer;
		size_t m_position;
		size_t m_maxsize;

};


