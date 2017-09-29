

class UDPMCastClient : public Thread {
    public:
        UDPMCastClient();
        ~UDPMCastClient();

        void Start();
        void Stop();
        void Run();
        void Restart();

        void SetPort(int port);
        void SetIP4Group(const std::string &str);
        void SetIP6Group(const std::string &str);

    protected:

        virtual void OnMessage(const std::string &host, const std::string &str) = 0;
        virtual void OnError(const std::string &msg) = 0;

    private:
        Mutex m_mutex;
        volatile bool m_running;
        int m_port;
        int m_eventfd;
        std::string m_ip4_group;
        std::string m_ip6_group;
};
