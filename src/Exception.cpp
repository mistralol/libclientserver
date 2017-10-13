
#include <stdarg.h>

#include <libclientserver.h>

Exception::Exception(const std::string &msg) {
    m_message = msg;
}

Exception::Exception(const char *fmt, ...) {
    char *tmp = 0;
    va_list ap;
    va_start(ap, fmt);
    int res = vasprintf(&tmp, fmt, ap);
    if (res < 0)
            abort();
    m_message = tmp;
    free(tmp);
    va_end(ap);
}

Exception::~Exception() {

}

const char *Exception::what() const throw() {
    return m_message.c_str();
}

