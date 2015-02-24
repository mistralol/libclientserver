
class Selector;

class ISelectable
{
	public:
		virtual ~ISelectable() { }

		virtual bool CanRead(const Selector *) = 0;
		virtual bool CanWrite(const Selector *) = 0;
		virtual bool CanExcept(const Selector *) = 0;
		virtual bool CanTimeout(const Selector *) = 0;

		virtual bool DoRead(Selector *) = 0;
		virtual bool DoWrite(Selector *) = 0;
		virtual bool DoExcept(Selector *) = 0;
		virtual bool DoTimeout(Selector *) = 0;

		virtual int GetFD(const Selector *) = 0;
		virtual void GetTimeout(const Selector *, struct timespec *tv)
		{
			tv->tv_sec = 0;
			tv->tv_nsec = 0;
		}
};


