
class ScopedReadLock
{
	public:
		ScopedReadLock(RWLock *lock);
		~ScopedReadLock();

		void Unlock();

	private:
		RWLock *m_lock;
		bool m_locked;
};

