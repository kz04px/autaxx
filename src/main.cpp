#include <iostream>
#include <string>
#include "protocol/protocol.hpp"

int main() {
    std::setbuf(stdin, NULL);
    std::setbuf(stdout, NULL);

    std::string input;
    std::cin >> input;

    if (input == "uai") {
        UAI::listen();
    } else if (input == "about") {
        std::cout << "Autaxx" << std::endl;
        std::cout << "Written in C++" << std::endl;
        std::cout << "Using libataxx" << std::endl;
    } else {
        std::cout << "Unknown protocol" << std::endl;
    }

    return 0;
}
