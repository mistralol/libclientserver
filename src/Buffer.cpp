
#include <libclientserver.h>

Buffer::Buffer()
{
	m_buffer_length = 8192;
	m_chunk_size = 65535;
	m_buffer_used = 0;
	m_buffer = NULL;
	m_max_size = 0;
}

Buffer::~Buffer()
{
	free(m_buffer);
}

/**
 * @Init
 *
 * One of the Init functions or overloads must be called before using any other function in the class
 */
int Buffer::Init()
{
	m_buffer = (char *) malloc(m_buffer_length * sizeof(m_buffer));
	if (!m_buffer)
		return -ENOMEM;
	return 0;
}

/**
 * @Init
 * @params[in] buflen This should be the initial size of memory allocated to the buffer
 *
 * One of the Init functions or overloads must be called before using any other function in the class
 */
int Buffer::Init(size_t buflen)
{
	m_buffer_length = buflen;
	return Init();
}

/**
 * @Read
 * @params[in] fd This should be the file descriptor to read from
 *
 * This function will attempt to read chunk size from the fd and populate the buffer contents with the data
 * If the buffer is going to exceed the max size it will return -ENOMEM
 * If the file has reached EOF -1 will be returned
 * If there is an error reading from the fd the approiate error code will be returned to indicte what the problem was.
 * In normal working cases without error's the length of the data read will be returned. This includes returning 0 if there was no data to read.
 */
int Buffer::Read(int fd)
{
	if (GetFreeSpace() < m_chunk_size)
	{
		if (ReSize(m_buffer_length + m_chunk_size) == false)
		{
			return -ENOMEM;
		}
	}

	int ret = read(fd, m_buffer + m_buffer_used, m_chunk_size);
	if (ret < 0)
	{
		switch(errno)
		{
			case EAGAIN:
			case EINTR:
				return 0;
			default:
				return -errno;
		}
	}
	if (ret == 0)
		return -1;
	m_buffer_used += ret;
	return ret;
}

/**
 * Write
 * @params[in] fd The file descriptor to write to
 *
 * This function will attempt to write as much of the buffer as possible to the fd.
 * If an error occurs it will will return the approate error code to indicate what the error was.
 * If the function was able to write any data it will return the amount of data that was written to the fd
 */
int Buffer::Write(int fd)
{
	if (m_buffer_used == 0)
		return 0;

	int ret = write(fd, m_buffer, m_buffer_used);
	if (ret > 0)
	{
		Shift(ret);
		return ret;
	}
	switch(errno)
	{
		case EAGAIN:
		case EINTR:
			return 0;
			break;
		default:
			return -errno;
			break;
	}
	return -1;
}

/**
 * WriteBuffered
 * @params[in] fd The file descriptor to write to
 * @params[in] buf The data to write
 * @params[in] length The amount of data to write from buf
 *
 * This functin will attempt to write as much data as it can to the file descriptor before it blocks.
 * Once it starts blocking it will then write the remaining part of the data to the buffer which can be written by calling Write(fd) later
 * If the initial write fails it will return the error that occured and will not attempt to write any data to either the fd or the buffer
 * If the buffers required more memory it is posisble for this function to file is there is a maximum buffer limit set. In which case it will fail with -NOBUFS
 * Under normal situations this function will always return the lgnth value passed
 */
int Buffer::WriteBuffered(int fd, const char *buf, size_t length)
{
	int ret = write(fd, buf, length);
	if (ret < 0)
	{
		switch(errno)
		{
			case EAGAIN:
			case EINTR:
				return PushData(buf, length);
			default:
				return -errno;
				break;
		}
	}
	size_t written = ret;

	if (written == length)
		return length;

	//Not all of it went to the socket to buffer it and say success to the caller
	ret = PushData(&buf[written], length - written);
	if (ret < 0)
		return ret;
	if ((size_t) ret != length - written)
		return -ENOBUFS;
	return length;
}

/**
 * PushData
 * @params[in] buf The buffer to append the to the buffer contents
 * @params[in] length The length of the data
 *
 * This function will append the data to the buffer
 * It is possible to fail with eother a memory allocation issue or if the maxmimum buffer size is going to be exceeded
 */
int Buffer::PushData(const char *buf, size_t length)
{
	if (GetFreeSpace() < length)
	{
		if (ReSize(m_buffer_length + length) == false)
		{
			return -ENOMEM;
		}
	}
	memcpy(&m_buffer[m_buffer_used], buf, length);
	m_buffer_used += length;
	return length;
}

/**
 * PullData
 * @params[in] buf This will be populated by the data
 * @params[in] length The size of the buffer buf
 *
 * This function will read data from the front of the buffer and populate as much as possible in buf
 * If the passed buf is larger than the data stored in the buffer then the return value will indicate how much data was copied ot buf
 */
int Buffer::PullData(char *buf, size_t length)
{
	int len = length;
	if (m_buffer_used < length)
		len = m_buffer_used;
	memcpy(buf, m_buffer, len);
	Shift(len);
	return len;
}

/*
 * GetLine
 * @params[in] str This will be modified to contain the next line found in the buffer
 *
 * This function will attempt to scan the buffer and return the first line of text found in the buffer
 * If it finds a line of text then true will be returned. It will return false if it the buffer does nto contain a full line
 */
bool Buffer::GetLine(std::string *str)
{
	return GetLine(str, '\n');
}

/*
 * GetLine
 * @params[in] str This will be modified to contain the next line found in the buffer
 * @params[in] term This will be used to specifiy the end of line marker to look for in the buffer
 *
 * This function will attempt to scan the buffer and return the first line of text found in the buffer
 * If it finds a line of text then true will be returned. It will return false if it the buffer does nto contain a full line
 */
bool Buffer::GetLine(std::string *str, const char term)
{
	if (GetDataLength() == 0)
		return false;

	char *lf = (char *) memchr(m_buffer, term, m_buffer_used);
	if (lf == NULL)
		return false;
	*lf = 0; //Add NULL Terminator

	*str = m_buffer;

	size_t Offset = lf - m_buffer + 1;
	Shift(Offset);
	return true;
}

/*
 * GetLine
 * @params[in] str This will be modified to contain the next line found in the buffer
 * @params[in] ending This will be used to specifiy the end of line marker to look for in the buffer
 *
 * This function will attempt to scan the buffer and return the first line of text found in the buffer
 * If it finds a line of text then true will be returned. It will return false if it the buffer does nto contain a full line
 */
bool Buffer::GetLine(std::string *str, const std::string &ending)
{
	return GetLine(str, ending.c_str());
}

/*
 * GetLine
 * @params[in] str This will be modified to contain the next line found in the buffer
 * @params[in] ending This will be used to specifiy the end of line marker to look for in the buffer
 *
 * This function will attempt to scan the buffer and return the first line of text found in the buffer
 * If it finds a line of text then true will be returned. It will return false if it the buffer does nto contain a full line
 */
bool Buffer::GetLine(std::string *str, const char *ending)
{
	if (GetDataLength() == 0)
		return false;

	char *lf = (char *) memmem(m_buffer, m_buffer_used, ending, strlen(ending));
	if (lf == NULL)
		return false;
	*lf = 0; //Add NULL Terminator

	*str = m_buffer;

	size_t Offset = lf - m_buffer + 1;
	Shift(Offset);
	return true;
}

/**
 * Shift
 * @params[in] size The amount of data to remove from the front of the buffer
 *
 * This will rmeove the size of bytes from the front of the buffer. If you attempt to remove more data that exists in the buffer abort will be called.
 */
void Buffer::Shift(size_t size)
{
	if (size == 0)
		return;
	if (m_buffer_used == 0)
		abort();
	if (size > m_buffer_used)
		abort();
	if (size == m_buffer_used)
	{
		m_buffer_used = 0;
		return;
	}
	memmove(m_buffer, m_buffer + size, m_buffer_used);
	m_buffer_used -= size;
}

/**
 * GetPtr
 *
 * This function return a raw value to a buffer. Beaware that calling other functions can invalidate this pointer as the memory block may be relocated to a different area.
 */
void *Buffer::GetPtr()
{
	return m_buffer;
}

/**
 * GetDataLength
 *
 * This function will return the number of bytes stored in the buffer
 */
size_t Buffer::GetDataLength()
{
	return m_buffer_used;
}

/**
 * GetFreeSpace
 *
 * This function will return the number of free bytes in the buffer before the buffer would need to be expanded to use more data
 */
size_t Buffer::GetFreeSpace()
{
	return m_buffer_length - m_buffer_used;
}

/**
 * GetTotalSpace
 *
 * This function returns the total space of the allocated buffer
 */
size_t Buffer::GetTotalSpace()
{
	return m_buffer_length;
}

/**
 * SetChunkSize
 * @params[in] size Value to store
 *
 * After setting this value it will be used to perform io on the Read function when attempting to read data
 */
void Buffer::SetChunkSize(size_t size)
{
	m_chunk_size = size;
}

/**
 * GetChunkSize
 *
 * Opposite of SetChunkSize
 */
size_t Buffer::GetChunkSize()
{
	return m_chunk_size;
}

/**
 * SetMaxSize
 *
 * This will set the enforced limit of how big the buffer can grow.
 * This function can fail if a limit is placed but there is currently data in the buffer bigger than the new limit
 * 0 will impose no limit
 */
int Buffer::SetMaxSize(size_t size)
{
	if (m_buffer_length > size)
	{
		if (Shrink() == false)
		{
			return -1;
		}
	}
	if (m_buffer_length > size)
	{
		return -1;
	}
	m_max_size = size;
	return 0;
}

/**
 * GetMaxSize
 *
 * This will return the maximum size the buffer can be.
 * 0 imposes no limit
 */
size_t Buffer::GetMaxSize()
{
	return m_max_size;
}

/**
 * Shrink
 *
 * Cause the allocated memory to be shrunk to use the minimum amount of memory
 */
bool Buffer::Shrink()
{
	return ReSize(m_buffer_used);
}

/**
 * ReSize
 *
 * Cause the allocated buffer memory to be resized
 */
bool Buffer::ReSize(size_t newsize)
{
	if (newsize < m_buffer_used)
		abort();
	if (m_max_size != 0 && newsize > m_max_size)
		return false;

	char *nbuf = (char *) realloc(m_buffer, newsize * sizeof(m_buffer));
	if (!nbuf)
		return false;
	m_buffer = nbuf;
	m_buffer_length = newsize;
	return true;
}

