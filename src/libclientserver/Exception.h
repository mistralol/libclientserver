
class Exception : public std::exception {
    public:
        Exception(const std::string &msg);
        Exception(const char *fmt, ...);

        ~Exception();

        virtual const char *what() const throw();

    private:
        std::string m_message;
};
