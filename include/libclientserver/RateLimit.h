

class RateLimit
{
	public:
		RateLimit();
		~RateLimit();

		void SetTimeout(const struct timespec *tv);

		bool Check();

	private:
		struct timespec m_timeout;
		struct timespec m_last;

};

