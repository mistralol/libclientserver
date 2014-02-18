
/**
 * @class ServerExcetpion
 * @brief exception which is suitable for throwing error while processing COMMANDs or REQUESTs
 *
 * Exception which is suitable for throwing error while processing COMMANDs or REQUESTs
 *
 */

class ServerException : public std::exception
{
	public:
		ServerException();
		ServerException(const int err);
		ServerException(const int err, const std::string Message);

		virtual ~ServerException() throw();

		virtual const char *what() const throw();

		int GetErrorNo() const;
		std::string GetErrorMessage() const;

	private:
		int m_errno;
		std::string m_msg;
};

