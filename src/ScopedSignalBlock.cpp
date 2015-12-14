
#include <libclientserver.h>

ScopedSignalBlock::ScopedSignalBlock(SignalHandler *handler) :
	m_handler(handler),
	m_blocked(false)
{
	handler->Block();
	m_blocked = true;
}

ScopedSignalBlock::~ScopedSignalBlock()
{
	if (m_blocked)
	{
		m_handler->UnBlock();
		m_blocked = false;
	}
}

void ScopedSignalBlock::Block()
{
	if (m_blocked == false)
	{
		m_handler->Block();
		m_blocked = true;
	}
}

void ScopedSignalBlock::UnBlock()
{
	if (m_blocked)
	{
		m_handler->UnBlock();
		m_blocked = false;
	}
}

