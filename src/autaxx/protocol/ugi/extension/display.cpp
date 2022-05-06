#include "display.hpp"
#include <iostream>

namespace UGI::Extension {

// Display the board
void display(const libataxx::Position &pos) {
    std::cout << pos << std::endl;
    std::cout << pos.get_fen() << std::endl;
    std::cout << pos.hash() << std::endl;
}

}  // namespace UGI::Extension
