#include <libclientserver.h>

ThreadFunction::ThreadFunction(std::function<void()> func, int interval) :
    m_func(func),
    m_interval(interval)
{
    m_func = func;
    m_interval = interval;
}

void ThreadFunction::Start() {
    m_running = true;
    Thread::Start();

}

void ThreadFunction::Stop() {
    ScopedLock lock(&m_mutex);
    m_running = false;
    m_mutex.WakeUp();
    lock.Unlock();
    Thread::Stop();
}

void ThreadFunction::WakeUp() {
    ScopedLock lock(&m_mutex);
    m_mutex.WakeUp();
}

ThreadFunction::~ThreadFunction() {
    if (m_running) {
        Stop();
    }
}

void ThreadFunction::Run() {
    ScopedLock lock(&m_mutex);
    while(m_running) {
        m_func();

        if (m_interval) {
            struct timespec ts = { m_interval, 0 };
            m_mutex.Wait(&ts);
        } else {
            m_mutex.Wait();
        }
    }
}
