
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

		void Add(RequestMapEntry *Entry);
		void Remove(RequestMapEntry *Entry);
		bool Exists(RequestMapEntry *Entry);

		bool IsComplete(RequestMapEntry *Entry);

		bool Wait(RequestMapEntry *Entry, const struct timespec *SoftTimeout, const struct timespec *HardTimeout);
		bool WakeUp(Json::Value &response);
		bool KeepAlive(Json::Value &response);

	private:
		std::map<int, RequestMapEntry *> m_map;
		Mutex m_Mutex;
};

