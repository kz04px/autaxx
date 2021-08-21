#include "display.hpp"
#include <iostream>

namespace UAI::Extension {

// Display the board
void display(const libataxx::Position &pos) {
    std::cout << pos << std::endl;
}

}  // namespace UAI::Extension
