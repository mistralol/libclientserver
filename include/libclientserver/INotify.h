

class INotify
{
	public:
		virtual ~INotify() { };

		virtual void Notify(void *) = 0;

};

