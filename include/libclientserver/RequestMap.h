

class RequestMap
{
	public:
		RequestMap();
		~RequestMap();

		void Add(RequestMapEntry *Entry);
		void Remove(RequestMapEntry *Entry);
		void Exists(RequestMapEntry *Entry);

		void Wait(RequestMapEntry *Entry);
		void WakeUp(RequestMapEntry *Entry);
		void KeepAlive(RequestMapEntry *Entry);

	private:
		std::map<uint64_t, RequestMapEntry> m_map;

};

