
class RequestMap
{
	public:
		RequestMap();
		~RequestMap();

		void Add(struct RequestMapEntry *Entry);
		void Remove(struct RequestMapEntry *Entry);
		bool Exists(struct RequestMapEntry *Entry);

		void Wait(struct RequestMapEntry *Entry, const struct timespec *SoftTimeout, const struct timespec *HardTimeout);
		void WakeUp(uint64_t id, const std::string *str);
		void KeepAlive(uint64_t id);

	private:
		std::map<uint64_t, struct RequestMapEntry *> m_map;
		Mutex m_Mutex;
};

