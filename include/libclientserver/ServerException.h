
class ServerException : public std::exception
{
	public:
		ServerException();
		ServerException(const std::string Message);
		ServerException(const int err);

		virtual ~ServerException() throw();

		virtual const char *what() const throw();

	private:
		std::string m_msg;
};

