
#include <libclientserver.h>

class UDPClient : public UDPMCastClient {
public:
    UDPClient() :
        HasMessage(false)
    {
        SetPort(1232);
        SetIP4Group("225.0.0.1");
        SetIP6Group("ff01::1");
        UDPMCastClient::Start();
    }

    ~UDPClient() {
        UDPMCastClient::Stop();
    }

    bool HasMessage;

    void OnMessage(const std::string &Host, const std::string &Msg) {
        HasMessage = true;
        printf("Host: %s Msg: %s\n", Host.c_str(), Msg.c_str());
    }

    void OnError(const std::string &str) {
        printf("Error: %s\n", str.c_str());
    }

};


int main(int argc, char **argv) {
    UDPClient C;

    int ret = system("./TestUDPMCast");
    printf("ret: %d", ret);

    if (!C.HasMessage)
        return EXIT_FAILURE;

    return 0;
}
