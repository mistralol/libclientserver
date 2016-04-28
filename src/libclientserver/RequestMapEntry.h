

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
		int id;
		Json::Value *Response;
		bool ValidResponse;
		bool KeepAlive;
};


