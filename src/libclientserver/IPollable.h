
class Poller;

/** 
 * @class IPollable
 * @brief Interface to be used with Poller
 *
 * Provides a number of call backs for dealying with Poller
 *
 */

class IPollable
{
	public:
		virtual ~IPollable() { }

		virtual bool CanRead(const Poller *) = 0;
		virtual bool CanWrite(const Poller *) = 0;
		virtual bool CanTimeout(const Poller *) = 0;

		virtual void DoRead(Poller *) = 0;
		virtual void DoWrite(Poller *) = 0;
		virtual void DoTimeout(Poller *) = 0;
		virtual void DoClose(Poller *) = 0;

		virtual int GetFD(const Poller *) = 0;
		virtual void GetTimeout(const Poller *, struct timespec *tv)
		{
			tv->tv_sec = 0;
			tv->tv_nsec = 0;
		}
};


