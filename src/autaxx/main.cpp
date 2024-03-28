#include <iostream>
#include <string>
#include "protocol/protocol.hpp"
#include "version.hpp"

int main() {
    std::setbuf(stdin, NULL);
    std::setbuf(stdout, NULL);

    std::string input;
    std::cin >> input;

    if (input == "uai") {
        UAI::listen();
    } else if (input == "ugi") {
        UGI::listen();
    } else if (input == "about") {
        std::cout << "Autaxx " << version_major << "." << version_minor << std::endl;
        std::cout << "Written in C++" << std::endl;
        std::cout << "Using libataxx" << std::endl;
        std::cout << "Date " << __DATE__ << std::endl;
        std::cout << "Time " << __TIME__ << std::endl;
#ifndef NDEBUG
        std::cout << "Debug build" << std::endl;
#endif
    } else {
        std::cout << "Unknown protocol" << std::endl;
    }

    return 0;
}
