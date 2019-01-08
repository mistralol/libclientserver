
#include <iostream>
#include <libclientserver.h>

int main(int argc, char **argv) {
    TaskPool Pool;

    auto f1 = Pool.Add([] { return 1; });
    auto f2 = Pool.Add([] { return 2; });
    auto f3 = Pool.Add([] { return 3; });
    auto f4 = Pool.Add([] { return 4; });
    std::cout << f1.get() << f2.get() << f3.get() << f4.get() << std::endl;

    return 0;
}

