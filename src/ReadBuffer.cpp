

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <libclientserver.h>

ReadBuffer::ReadBuffer(size_t MaxSize)
{
	m_maxsize = MaxSize;
	m_position = 0;
	m_buffer = (char *) malloc(MaxSize * sizeof(*m_buffer));
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
	if (m_maxsize - m_position == 0)
		abort();	//FIXME: Buffer Going to be Exceeded

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
	return false;
}

void ReadBuffer::Shift(size_t offset)
{
	if (m_position == 0)
		return;
	memmove(m_buffer, &m_buffer[offset], m_position - offset);
	m_position = 0;
}



