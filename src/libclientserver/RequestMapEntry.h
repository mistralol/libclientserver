

class RequestMapEntry
{
	public:
		RequestMapEntry()
		{
			id = 0;
			Response = NULL;
			ValidResponse = false;
			KeepAlive = false;
		}

	public:
		uint64_t id;
		Request *Response;
		bool ValidResponse;
		bool KeepAlive;
};


