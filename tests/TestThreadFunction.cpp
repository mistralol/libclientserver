#include <libclientserver.h>

bool Worked = false;

class MyClass {
    public:
        void Run() {
            Worked = true;
            printf("Worked\n");
        }
};

int main(int argc, char **argv) {
    MyClass MC;
    ThreadFunction TFunc(std::bind(&MyClass::Run, &MC));

    TFunc.Start();

    TFunc.Stop();

    if (!Worked) {
        return EXIT_FAILURE;
    }

    return 0;
}
