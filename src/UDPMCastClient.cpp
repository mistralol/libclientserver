
#include <libclientserver.h>

UDPMCastClient::UDPMCastClient() :
    m_running(false),
    m_port(0),
    m_eventfd(-1)
{

}

UDPMCastClient::~UDPMCastClient() {

}

void UDPMCastClient::Start() {
    ScopedLock lock(&m_mutex);
    m_running = true;
    m_eventfd = eventfd(0, 0);
    if (m_eventfd < 0)
        throw(std::runtime_error("UDPMCastClient::Start -> Cannot open eventfd"));
    Thread::Start();
}

void UDPMCastClient::Stop() {
    ScopedLock lock(&m_mutex);
    if (!m_running)
        abort();
    m_running = false;
    uint64_t data = 1;
    if (write(m_eventfd, &data, sizeof(data)) < 0)
        abort();
    lock.Unlock();
    Thread::Stop();
    if (close(m_eventfd) < 0)
        abort();
}

void UDPMCastClient::Run() {
    ScopedLock lock(&m_mutex);
    while(m_running) {
        lock.Unlock();
        int port = m_port;
        std::string IP4Group = m_ip4_group;
        std::string IP6Group = m_ip6_group;

        int fd = socket(AF_INET, SOCK_DGRAM, 0);
        if (fd < 0) {
            OnError("Cannot create udp socket");
            sleep(1);
            continue;
        }

        struct pollfd fds[2];
        int bufsize = 65535;
        char *buffer = (char *) malloc(bufsize);
        struct sockaddr_in addr;
        struct ip_mreq mreq;
        socklen_t addrlen = sizeof(addr);
        int ret = 0;

        memset(&addr, 0, sizeof(addr));
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = htonl(INADDR_ANY);
        addr.sin_port = htons(port);

        if (bind(fd, (struct sockaddr *) &addr, addrlen) < 0) {
            OnError("Cant bind to port");
            goto err;
        }
        
        mreq.imr_multiaddr.s_addr = inet_addr(IP4Group.c_str());
        mreq.imr_interface.s_addr = htonl(INADDR_ANY);
        if (setsockopt(fd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq)) < 0) {
            OnError("Cannot add to group membership");
            goto err;
        }

        do {
            fds[0].fd = m_eventfd;
            fds[0].events = POLLIN;
            fds[0].revents = 0;

            fds[1].fd = fd;
            fds[1].events = POLLIN;
            fds[1].revents = 0;

            ret = poll(fds, 2, 0);
            if (ret < 0) {
                switch(errno) {
                    case EINTR:
                        break;
                    default:
                        std::string err = strerror(errno);
                        OnError(err);
                        break;
                }
            } else {
                if ((fds[1].revents & POLLIN)) {
                    //Read
                    ret = recvfrom(fd, buffer, bufsize - 1, 0, (struct sockaddr *) &addr, &addrlen);
                    if (ret < 0) {
                        OnError("Read Error");
                    } else {
                        buffer[ret] = 0; //NULL Terminate string
                        std::string Host(inet_ntoa(addr.sin_addr));
                        std::string Msg(buffer);
                        OnMessage(Host, Msg);
                    }
                }
            }

        } while(m_running);

    err:
        free(buffer);

        if (close(fd) < 0)
            abort();
        if (m_running)
            sleep(1);
        lock.Lock();
    }
}

void UDPMCastClient::SetPort(int port) {
    ScopedLock lock(&m_mutex);
    m_port = port;
    if (m_running) {
        Stop();
        Start();
    }
}

void UDPMCastClient::SetIP4Group(const std::string &str) {
    ScopedLock lock(&m_mutex);
    m_ip4_group = str;
    if (m_running) {
        Stop();
        Start();
    }
}

void UDPMCastClient::SetIP6Group(const std::string &str) {
    ScopedLock lock(&m_mutex);
    m_ip6_group = str;
    if (m_running) {
        Stop();
        Start();
    }
}

