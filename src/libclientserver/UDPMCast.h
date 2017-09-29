
class UDPMCast : protected Thread {
    public:
        UDPMCast();
        ~UDPMCast();

        void Start();
        void Stop();
        void Run();

        void SetPort(int port);
        void SetInterval(const struct timespec &ts);
        void SetIP4Group(const std::string &str);
        void SetIP6Group(const std::string &str);
        void SetMessage(const std::string &str);

    private:
        Mutex m_mutex;
        int m_port;
        std::string m_ip4_group;
        std::string m_ip6_group;
        std::string m_message;
        struct timespec m_interval;
        volatile bool m_running;
};
