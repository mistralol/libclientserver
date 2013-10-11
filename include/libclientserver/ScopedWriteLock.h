
class ScopedWriteLock
{
	public:
		ScopedWriteLock(RWLock *lock);
		~ScopedWriteLock();

		void Unlock();

	private:
		RWLock *m_lock;
		bool m_locked;
};

