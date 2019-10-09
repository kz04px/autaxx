#include "display.hpp"
#include <iostream>

namespace UAI {

namespace Extension {

// Display the board
void display(const libataxx::Position &pos) {
    std::cout << pos << std::endl;
}

}  // namespace Extension

}  // namespace UAI
