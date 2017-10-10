#include <libclientserver.h>

class MyClass {
    public:
        void Run() {
            printf("Worked\n");
        }
};

int main(int argc, char **argv) {
    MyClass MC;
    ThreadFunction TFunc(std::bind(&MyClass::Run, &MC));

    TFunc.Start();

    TFunc.Stop();

    return 0;
}
