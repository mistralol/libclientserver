
//Note: SQueue can only return NULL when Flush() is called which
//can be used to the mark the end of the queue

template <class T>
class SQueue
{
	public:
		SQueue() :
			m_maxsize(0),
			m_hwsize(0),
			m_count(0)
		{

		}

		~SQueue()
		{
			Flush();
		}

		void Add(T item)
		{
			ScopedLock lock(&m_mutex);
			if (m_maxsize != 0 && m_queue.size() >= m_maxsize)
				throw(std::runtime_error("Queue Full"));

			if (m_queue.size() > m_hwsize)
				m_hwsize = m_queue.size();

			m_count++;
			m_queue.push_back(item);
			lock.Unlock();
			m_sem.Up();
		}

		void GetNext(T &val)
		{
			m_sem.Down();
			ScopedLock lock(&m_mutex);
			if (m_queue.empty() == true)
			{
				m_mutex.WakeUp(); //WakeUp during flush
				throw(std::runtime_error("No Queue Item"));
			}
			val = m_queue.front();
			m_queue.pop_front();
		}

		void Flush()
		{
			ScopedLock lock(&m_mutex);
			while(m_queue.empty() == false)
			{
				m_mutex.Wait();
			}
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
		Semaphore m_sem;
		size_t m_maxsize;
		size_t m_hwsize; //High water mark for size
		uint64_t m_count; //Number of items processed
		std::list<T> m_queue;
		Mutex m_mutex;
};

