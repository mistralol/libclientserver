
#include <libclientserver.h>

int main(int argc, char **argv) {
    UDPMCast C;
    struct timespec ts = {1, 0};

    C.SetInterval(ts);
    C.SetMessage("Hello World");
    C.SetIP4Group("225.0.0.1");
    C.SetIP6Group("ff01::1");
    C.SetPort(1232);
    C.Start();

    sleep(5);

    C.Stop();

    return 0;
}
