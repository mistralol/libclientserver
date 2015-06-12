

template <class T>
class Queue
{
	public:
		Queue()
		{
			m_maxsize = 0;
			m_hwsize = 0;
			m_flushing = false;
		}

		~Queue()
		{
			Flush(); //This prevents the queue from being destryoed when it contains items
		}

		bool Add(T item)
		{
			ScopedLock lock(&m_mutex);
			if (m_maxsize != 0 && m_queue.size() >= m_maxsize)
				return false;

			if (m_queue.size() > m_hwsize)
				m_hwsize = m_queue.size();

			m_count++;
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
					//Since we are removing items. If the queue is being flushed also do a wakeup
					if (m_flushing && m_queue.empty() == true)
						m_mutex.WakeUp();
					return tmp;
				}
				else
				{
					if (m_mutex.Wait(ts) == -ETIMEDOUT)
						return NULL;
				}
			}
			return NULL;
		}

		void Flush()
		{
			ScopedLock lock(&m_mutex);
			m_flushing = true;
			while(m_queue.empty() == false)
			{
				struct timespec ts = {1, 0};
				m_mutex.Wait(&ts);
			}
			m_flushing = false;
		}

		size_t GetCount()
		{
			ScopedLock lock(&m_mutex);
			return m_queue.size();
		}

		size_t GetHWCount()
		{
			ScopedLock lock(&m_mutex);
			return m_hwsize;
		}

		uint64_t GetProcCount()
		{
			ScopedLock lock(&m_mutex);
			return m_count;
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
		size_t m_hwsize; //High water mark for size
		uint64_t m_count; //Number of items processed
		bool m_flushing;
		std::list<T> m_queue;
		Mutex m_mutex;
};

