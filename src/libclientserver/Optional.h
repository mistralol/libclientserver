

template <class T>
class Optional
{
	public:
		Optional() :
			m_isvalid(false)
		{
		
		}
		
		Optional(T data) :
			m_isvalid(true)
		{
			m_data = data;
		}
		
		void Reset()
		{
			m_isvalid = false;
		}
		
		void Reset(T data)
		{
			m_isvalid = true;
			m_data = data;
		}
		
		T get()
		{
			if (!m_isvalid)
				abort();
			return m_data;
		}
		
		operator bool() const
		{
			return m_isvalid;
		}

	private:
		bool m_isvalid;
		T m_data;
};

