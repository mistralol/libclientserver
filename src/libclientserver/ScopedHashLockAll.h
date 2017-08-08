
template <class T> class ScopedHashLockAll {
    public:
        ScopedHashLockAll(HashMutex<T> *mutex) {
            m_mutex = mutex;
            m_mutex->LockAll();
            m_locked = true;
        }

        ~ScopedHashLockAll() {
            if (m_locked) {
                m_mutex->UnlockAll();
                m_locked = false;
            }
        }

        void Unlock() {
            if (!m_locked)
                abort();
            m_mutex->UnlockAll();
            m_locked = false;
        }


    private:
        HashMutex<T> *m_mutex;
        bool m_locked;

};
