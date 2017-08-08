
template <class T> class HashMutex {
    public:
        HashMutex() {

        }

        ~HashMutex() {
            if (!m_map.empty()) {
                abort();
            }
        }

        void Lock(T key) {
            ScopedLock lock(&m_mutex);
            do {
                auto it = m_map.find(key);
                if (it == m_map.end()) {
                    //We Won!
                    m_map[key] = 0; //Initial Usage Count of 0
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
        }

        void LockAll() {
            m_mutex.Lock();
        }

        void UnlockAll() {
            m_mutex.Unlock();
        }

    private:
        Mutex m_mutex;
        std::map<T, int> m_map;   //Key, Usage

};
