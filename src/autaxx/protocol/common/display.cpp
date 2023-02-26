#include "display.hpp"
#include <iostream>

namespace common {

// Display the board
void display(const libataxx::Position &pos) {
    std::cout << pos << std::endl;
    std::cout << pos.get_fen() << std::endl;
    std::cout << std::hex << "0x" << pos.hash() << std::dec << std::endl;
}

}  // namespace common
