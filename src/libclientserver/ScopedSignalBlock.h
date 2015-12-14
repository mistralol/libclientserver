
class ScopedSignalBlock
{
	public:
		ScopedSignalBlock(SignalHandler *handler);
		~ScopedSignalBlock();
		
		void Block();
		void UnBlock();
	private:
		SignalHandler *m_handler;
		bool m_blocked;
};

