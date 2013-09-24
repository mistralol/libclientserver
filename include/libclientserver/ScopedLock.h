
class ScopedLock {
	public:
		ScopedLock(Mutex *m);
		~ScopedLock();

		private:
			Mutex *m_mutex;
};


