

template <class T>
class Queue
{
	public:
		Queue()
		{
			m_maxsize = 0;
		}

		~Queue()
		{
			Flush();
		}

		bool Add(T item)
		{
			ScopedLock lock(&m_mutex);
			if (m_maxsize != 0 && m_queue.size() >= m_maxsize)
				abort();
			m_queue.push_back(item);
			m_mutex.WakeUp();
			return true;
		}

		T GetNext()
		{
			void *tmp = NULL;
			ScopedLock lock(&m_mutex);
			while(tmp == NULL)
			{
				if (m_queue.empty() == false)
				{
					tmp = m_queue.front();
					m_queue.pop_front();
					return tmp;
				}
				else
				{
					m_mutex.Wait();
				}
			}
			return NULL;
		}

		T GetNext(struct timespec *ts)
		{
			T tmp = NULL;
			ScopedLock lock(&m_mutex);
			while(tmp == NULL)
			{
				if (m_queue.empty() == false)
				{
					tmp = m_queue.front();
					m_queue.pop_front();
					return tmp;
				}
				else
				{
					m_mutex.Wait(ts);
				}
			}
			return NULL;
		}

		void Flush()
		{
			m_mutex.Lock();
			while(m_queue.empty() == false)
			{
				m_mutex.Unlock();
				sleep(1);	//FIXME: Remove sleep
				m_mutex.Lock();
			}
			m_mutex.Unlock();
		}

		size_t GetCount()
		{
			ScopedLock lock(&m_mutex);
			return m_queue.size();
		}

		void SetMaxItems(size_t size)
		{
			ScopedLock lock(&m_mutex);
			m_maxsize = size;
		}

		size_t GetMaxItems(size_t size)
		{
			ScopedLock lock(&m_mutex);
			return m_maxsize;
		}

	private:
		size_t m_maxsize;
		std::list<T> m_queue;
		Mutex m_mutex;
};

