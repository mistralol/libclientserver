

class Notify
{
	public:
		Notify();
		~Notify();

		void Add(INotify *p);
		void Remove(INotify *p);

		void Call(void *d);

	private:
		std::map<INotify *, INotify *> m_map;
		Mutex m_mutex;
};


