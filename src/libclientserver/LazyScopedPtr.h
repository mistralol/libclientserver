
template<class T>
class LazyScopedPtr
{
	public:
		LazyScopedPtr(T *(*func) ())
			: m_p(NULL)
		{
			m_func = func;
		}
		
		~LazyScopedPtr()
		{
			if (m_p)
				delete m_p;
		}
		
		T *operator->() const
		{
			if (!m_p)
				m_p = m_func();
			return m_p;
		}
	
	private:
		T *(*m_func)();
		mutable T *m_p;
};

