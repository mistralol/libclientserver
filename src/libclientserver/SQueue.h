
//Note: SQueue can only return NULL when Flush() is called which
//can be used to the mark the end of the queue

template <class T>
class SQueue
{
	public:
		SQueue()
		{
			m_sem = new Semaphore();
			m_maxsize = 0;
			m_hwsize = 0;
		}

		~SQueue()
		{
			Flush();
			delete m_sem;
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
			lock.Unlock();
			m_sem->Up();
			return true;
		}

		T GetNext()
		{
			m_sem->Down();
			T tmp = NULL;
			ScopedLock lock(&m_mutex);
			if (m_queue.empty() == true)
				return NULL;
			tmp = m_queue.front();
			m_queue.pop_front();
			return tmp;
		}

		void Flush()
		{
			m_sem->Up();
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
		Semaphore *m_sem;
		size_t m_maxsize;
		size_t m_hwsize; //High water mark for size
		uint64_t m_count; //Number of items processed
		std::list<T> m_queue;
		Mutex m_mutex;
};

