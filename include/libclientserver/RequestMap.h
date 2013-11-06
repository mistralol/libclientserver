
/**
 * @class RequestMap
 * @brief Providing tracking of Requests
 *
 * This class will provide a means of tracking a request. It is typically used in the following way.
 * 1. Add item to request map.
 * 2. Send request to server
 * 3. Wait / Sleep for the response.
 *
 * A seperate thread is typially used to wake the sleeping process.
 * 1. Read data from socket
 * 2. Wakup the thread that is Waiting for a response
 */

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

