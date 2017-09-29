
#include <libclientserver.h>
#include <ifaddrs.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

UDPMCast::UDPMCast() {

}

UDPMCast::~UDPMCast() {

}

void UDPMCast::Start() {
    ScopedLock lock(&m_mutex);
    if (m_running)
        abort();
    m_running = true;
    Thread::Start();
}

void UDPMCast::Stop() {
    ScopedLock lock(&m_mutex);
    if (m_running == false)
        abort();
    m_running = false;
    m_mutex.WakeUpAll();
    m_mutex.Unlock();
    Thread::Stop();
}

void UDPMCast::Run() {
    ScopedLock lock(&m_mutex);
    while(m_running) {
        //Copy local so we can run unlocked
        int port = m_port;
        std::string IP4Group = m_ip4_group;
        std::string IP6Group = m_ip6_group;
        std::string Message = m_message;
        lock.Unlock();

        do {
            struct ifaddrs *ifaddr, *ifa;
            if (getifaddrs(&ifaddr) == -1) {
                perror("getifaddrs");
                abort();
            }
            ifa = ifaddr;
            while(ifa) {
                int fd = -1;
                struct sockaddr_in addr;
                struct sockaddr_in6 addr6;
                int addrsize = (ifa->ifa_addr->sa_family == AF_INET) ? sizeof(struct sockaddr_in) : sizeof(struct sockaddr_in6);
                if (!(ifa->ifa_addr->sa_family == AF_INET || ifa->ifa_addr->sa_family == AF_INET6))
                    goto skip;

                fd = socket(ifa->ifa_addr->sa_family, SOCK_DGRAM, 0);
                if (fd < 0) {
                    abort();
                    goto skip;
                }
    
                //send
                if (ifa->ifa_addr->sa_family == AF_INET && IP4Group != "") {
    
                    if (bind(fd, ifa->ifa_addr, addrsize) < 0) {
                        goto skip;
                    }
    
                    memset(&addr,0,sizeof(addr));
                    addr.sin_family = AF_INET;
                    addr.sin_addr.s_addr = inet_addr(IP4Group.c_str());
                    addr.sin_port = htons(port);
    
                    if (sendto(fd, Message.c_str(), Message.size(), 0, (struct sockaddr *) &addr, sizeof(addr)) < 0) {
                        //perror("sendto");
                    }
                } else if (ifa->ifa_addr->sa_family == AF_INET6 && IP6Group != "") {
                    if (bind(fd, ifa->ifa_addr, addrsize) < 0) {
                        goto skip;
                    }
    
                    memset(&addr6, 0, sizeof(addr6));
                    addr6.sin6_family = AF_INET6;
                    if (inet_pton(AF_INET6, IP6Group.c_str(), &addr6.sin6_addr) != 1) {
                        abort();
                    }
                    addr6.sin6_port = htons(port);
    
                    if (sendto(fd, Message.c_str(), Message.size(), 0, (struct sockaddr *) &addr, sizeof(addr)) < 0) {
                        //perror("sendto");
                    }
                }
    
        skip:
                if (fd >= 0)
                    close(fd);
                ifa = ifa->ifa_next;
            }
    
            freeifaddrs(ifaddr);

        } while(0);

        lock.Lock();
        if (!m_running)
            break;
        m_mutex.Wait(&m_interval);
    }
}

void UDPMCast::SetPort(int port) {
    ScopedLock lock(&m_mutex);
    m_port = port;
}

void UDPMCast::SetInterval(const struct timespec &ts) {
    ScopedLock lock(&m_mutex);
    m_interval = ts;
    m_mutex.WakeUpAll();
}

void UDPMCast::SetIP4Group(const std::string &str) {
    ScopedLock lock(&m_mutex);
    m_ip4_group = str;
}

void UDPMCast::SetIP6Group(const std::string &str) {
    ScopedLock lock(&m_mutex);
    m_ip6_group = str;
}

void UDPMCast::SetMessage(const std::string &str) {
    ScopedLock lock(&m_mutex);
    m_message = str;
}
