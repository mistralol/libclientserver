
class ThreadFunction : public Thread {
    public:
        ThreadFunction(std::function<void()>, int seconds = 0);
        ~ThreadFunction();
    
        void Start();
        void Stop();
        void WakeUp();
        void Run();

    private:
        std::function<void()> m_func;
        Mutex m_mutex;
        int m_interval;
        bool m_running;
};

