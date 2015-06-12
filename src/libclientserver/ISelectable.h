
class Selector;

/** 
 * @class ISelectable
 * @brief Interface to be used with Selector
 *
 * Provides a number of call backs for dealying with Selector
 *
 */

class ISelectable
{
	public:
		virtual ~ISelectable() { }

		virtual bool CanRead(const Selector *) = 0;
		virtual bool CanWrite(const Selector *) = 0;
		virtual bool CanExcept(const Selector *) = 0;
		virtual bool CanTimeout(const Selector *) = 0;

		virtual void DoRead(Selector *) = 0;
		virtual void DoWrite(Selector *) = 0;
		virtual void DoExcept(Selector *) = 0;
		virtual void DoTimeout(Selector *) = 0;
		virtual void DoClose(Selector *) = 0;

		virtual int GetFD(const Selector *) = 0;
		virtual void GetTimeout(const Selector *, struct timespec *tv)
		{
			tv->tv_sec = 0;
			tv->tv_nsec = 0;
		}
};


