#ifndef COMMON_HPP
#define COMMON_HPP

#include <libataxx/position.hpp>
#include <sstream>

namespace common {

// Set the current position
// -- position startpos
// -- position startpos moves a3 d2d4
// -- position fen x5o/7/2-1-2/7/2-1-2/7/o5x x 0 1
// -- position fen x5o/7/2-1-2/7/2-1-2/7/o5x x 0 1 moves a3 d2d4
void position(libataxx::Position &pos, std::stringstream &stream);

}  // namespace common

#endif
