
//Warning: This need to be used before any other threads are started!
class SignalHandler : Thread
{
	public:
		SignalHandler(ISignalHandler *handler);
		~SignalHandler();

		void Block();
		void UnBlock();

	protected:
		void Run();

	private:
		ISignalHandler *handler;
		Mutex m_mutex;
		bool m_loop;
		sigset_t m_sigs;
};

