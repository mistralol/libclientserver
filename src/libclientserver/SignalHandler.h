
//Warning: This need to be initilized before any other threads are started!
//or any other intial threads will have an incorrect signal mask and may intercept some of the signals!
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
		ISignalHandler *m_handler;
		Mutex m_mutex;
		bool m_loop;
		sigset_t m_sigs;
};

