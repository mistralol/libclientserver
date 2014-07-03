
/**
 * @class Barrier
 * @brief Class To support a Barrier for thread sync
 *
 * When Wait is called the thread will block until another thread has called WakeUp
 * A call to WakeUp is only required once after that the Barrier will no longer block any calls to Wait
 *
 */
class Barrier
{
	public:
		Barrier();
		~Barrier();

		void Wait();
		bool Wait(const struct timespec *Timeout);

		void WakeUp();

	private:
		bool m_fired;
		Mutex m_mutex;

};

