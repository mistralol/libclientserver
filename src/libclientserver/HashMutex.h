

//This can suffer from lock starvation when taking globla locks.
//You have be warned. Be careful with it!!

template <class T> class HashMutex {
    public:
        HashMutex() : m_blocknew(0), m_locks(0) {

        }

        ~HashMutex() {
            if (!m_map.empty()) {
                abort();
            }
        }

        void Lock(T key) {
            ScopedLock lock(&m_mutex);

            //Wait if there is a global lock on this
            while(m_blocknew) {
                m_mutex.Wait();
            }

            do {
                auto it = m_map.find(key);
                if (it == m_map.end()) {
                    //We Won!
                    m_map[key] = 0; //Initial Usage Count of 0
                    m_locks++;
                    return;
                } else {
                    m_map[key]++;
                    m_mutex.Wait();
                    m_map[key]--;
                }
            } while(1);
        }

        void Unlock(T key) {
            ScopedLock lock(&m_mutex);
            auto it = m_map.find(key);
            if (it->second == 0) {
                m_map.erase(it);
            } else {
                m_mutex.WakeUpAll();
            }
            m_locks--;
        }

        void LockAll() {
            m_mutex.Lock();
            m_blocknew++;  //Need to block new hash locks or we risk never getting the lock
            while(m_locks > 0) {
                m_mutex.Wait();
            }
            m_blocknew--;
        }

        void UnlockAll() {
            m_mutex.WakeUpAll();
            m_mutex.Unlock();
        }

    private:
        Mutex m_mutex;
        unsigned int m_blocknew;
        unsigned int m_locks;
        std::map<T, int> m_map;   //Key, Usage

};
