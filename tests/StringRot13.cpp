
#include <libclientserver.h>

static void Test(const std::string &str) {
    std::string Crypt = String::Rot13(str);
    if (Crypt == str) {
        printf("Failed1: %s == %s\n", str.c_str(), Crypt.c_str());
        abort();
    }

    std::string Original = String::Rot13(Crypt);
    if (Original != str) {
        printf("Failed2: %s != %s\n", str.c_str(), Original.c_str());
        abort();
    }
    printf("Passed: %s -> %s\n", str.c_str(), Crypt.c_str());
}

int main(int argc, char **argv)
{
    Test("Hello World");
    Test("SomeString");
    Test("a01234567890");
    Test("abcdefgh");
    Test("aAnN");

	return EXIT_SUCCESS;
}

