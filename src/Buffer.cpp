
#include <libclientserver.h>

Buffer::Buffer()
{
	m_buffer_length = 8192;
	m_chunk_size = 65535;
	m_buffer_used = 0;
	m_buffer = NULL;
}

Buffer::~Buffer()
{
	free(m_buffer);
}

int Buffer::Init()
{
	m_buffer = (char *) malloc(m_buffer_length * sizeof(m_buffer));
	if (!m_buffer)
		return -ENOMEM;
	return 0;
}

int Buffer::Init(size_t buflen)
{
	m_buffer_length = buflen;
	return Init();
}

int Buffer::Read(int fd)
{
	if (GetFreeSpace() < m_chunk_size)
		if (ReSize(m_buffer_length + m_chunk_size) == false)
			return -ENOMEM;

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

int Buffer::PushData(const char *buf, size_t length)
{
	if (GetFreeSpace() < length)
		if (ReSize(m_buffer_length + length) == false)
			return -ENOMEM;
	memcpy(&m_buffer[m_buffer_used], buf, length);
	m_buffer_used += length;
	return length;
}

int Buffer::PullData(char *buf, size_t length)
{
	int len = length;
	if (m_buffer_used < length)
		len = m_buffer_used;
	memcpy(buf, m_buffer, len);
	Shift(len);
	return len;
}

void Buffer::Shift(size_t size)
{
	if (size == 0)
		return;
	if (m_buffer_used == 0)
		abort();
	if (size == m_buffer_used)
	{
		m_buffer_used = 0;
		return;
	}
	memmove(m_buffer, m_buffer + size, m_buffer_used);
	m_buffer_used -= size;
}

void *Buffer::GetPtr()
{
	return m_buffer;
}

size_t Buffer::GetDataLength()
{
	return m_buffer_used;
}

size_t Buffer::GetFreeSpace()
{
	return m_buffer_length - m_buffer_used;
}

size_t Buffer::GetTotalSpace()
{
	return m_buffer_length;
}

void Buffer::SetChunkSize(size_t size)
{
	m_chunk_size = size;
}

size_t Buffer::GetChunkSize()
{
	return m_chunk_size;
}

void Buffer::Shrink()
{
	ReSize(m_buffer_used);
}

bool Buffer::ReSize(size_t newsize)
{
	if (newsize < m_buffer_used)
		abort();

	char *nbuf = (char *) realloc(m_buffer, newsize * sizeof(m_buffer));
	if (!nbuf)
		return false;
	m_buffer = nbuf;
	m_buffer_length = newsize;
	return true;
}

