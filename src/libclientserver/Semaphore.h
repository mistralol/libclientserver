

class Semaphore
{
	public:
		Semaphore();
		~Semaphore();
		
		void Up();
		void Down();
		int TryDown();
		int DownTimeout(const struct timespec *ts);
	private:
		sem_t m_sem;
};


