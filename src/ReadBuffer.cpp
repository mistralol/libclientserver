

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <libclientserver.h>

ReadBuffer::ReadBuffer(size_t Size)
{
	m_maxsize = Size;
	m_size = Size;
	m_position = 0;
	m_buffer = (char *) malloc(Size * sizeof(*m_buffer));
	if (!m_buffer)
		abort();
}

ReadBuffer::ReadBuffer(size_t Size, size_t MaxSize)
{
	m_maxsize = MaxSize;
	m_size = Size;
	m_position = 0;
	m_buffer = (char *) malloc(Size * sizeof(*m_buffer));
	if (!m_buffer)
		abort();
}

ReadBuffer::~ReadBuffer()
{
	free(m_buffer);
}

char *ReadBuffer::GetStartPtr()
{
	return m_buffer;
}

size_t ReadBuffer::GetPosition()
{
	return m_position;
}

int ReadBuffer::Read(int fd)
{
	//Make buffer large if it is required. This should be up to maxsize at which point reject it.
	if (m_maxsize - m_position == 0)
	{
		if (m_size < m_maxsize)
		{
			size_t newsize = m_size *= 2;	//Double buffer size
			if (newsize > m_maxsize)
				newsize = m_maxsize;		//Truncate it to max length
			if (newsize == m_size)
				return -ENOBUFS;			//Fail if we are already at maxlength

			char *tmp = (char *) realloc(m_buffer, newsize * sizeof(*m_buffer));
			if (!tmp)
				return -ENOMEM;
			m_buffer = tmp;
			m_size = newsize;
		}
	}

	int ret = read(fd, &m_buffer[m_position], m_maxsize - m_position);
	if (ret > 0)
	{
		m_position += ret;
	}
	if (ret < 0)
		return -errno;
	
	return ret;
}

bool ReadBuffer::GetLine(std::string *str)
{
	if (m_position == 0)
		return false;

	char *lf = (char *) memchr(m_buffer, '\n', m_position);
	if (lf == NULL)
		return false;
	*lf = 0; //Add NULL Terminator

	*str = m_buffer;

	size_t Offset = lf - m_buffer;
	Shift(Offset);
	return true;
}

void ReadBuffer::Shift(size_t offset)
{
	if (m_position == 0)
		return;
	memmove(m_buffer, &m_buffer[offset], m_position - offset);
	m_position = 0;
}



