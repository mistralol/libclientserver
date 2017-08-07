
template <class T> class ScopedHashLock {
    public:
        ScopedHashLock(HashMutex<T> *lock, T key) {
            m_mutex = lock;
            m_key = key;
            m_mutex->Lock(key);
            m_locked = true;
        }

        ~ScopedHashLock() {
            if (m_locked) {
                m_mutex->Unlock(m_key);
                m_locked = false;
            }
        }

        void Unlock() {
            if (!m_locked)
                abort();
            m_mutex->Unlock(m_key);
            m_locked = false;
        }


    private:
        HashMutex<T> *m_mutex;
        T m_key;
        bool m_locked;

};
