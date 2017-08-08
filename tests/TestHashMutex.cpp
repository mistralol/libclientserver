
#include <libclientserver.h>


int main(int argc, char **argv) {
    HashMutex<std::string> mutex;

    mutex.Lock("Key");
    mutex.Unlock("Key");

    mutex.Lock("Key");
    mutex.Unlock("Key");

    mutex.Lock("Key2");
    mutex.Unlock("Key2");

    ScopedHashLock<std::string> lock(&mutex, "Key");

    ScopedHashLockAll<std::string> lock2(&mutex);

    ScopedHashLockAll<std::string> lock3(&mutex);
    lock3.Unlock();

    return 0;
}
