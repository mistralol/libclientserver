

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

		void Add(T &item)
		{
			ScopedLock lock(&m_mutex);
			if (m_maxsize != 0 && m_queue.size() >= m_maxsize)
				throw(std::runtime_error("Queue Full"));

			if (m_queue.size() > m_hwsize)
				m_hwsize = m_queue.size();

			m_count++;
			m_queue.push_back(item);
			m_mutex.WakeUp();
		}

		void GetNext(T &val)
		{
			ScopedLock lock(&m_mutex);
			while(1)
			{
				if (m_queue.empty() == false)
				{
					val = m_queue.front();
					m_queue.pop_front();
					return;
				}
				else
				{
					m_mutex.Wait();
				}
			}
		}

		bool GetNext(struct timespec *ts, T &val)
		{
			bool found = false;
			ScopedLock lock(&m_mutex);
			while(found == false)
			{
				if (m_queue.empty() == false)
				{
					val = m_queue.front();
					m_queue.pop_front();
					//Since we are removing items. If the queue is being flushed also do a wakeup
					if (m_flushing && m_queue.empty() == true)
						m_mutex.WakeUp();
					return true;
				}
				else
				{
					if (m_mutex.Wait(ts) == -ETIMEDOUT)
						return false;
				}
			}
			return false;
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

