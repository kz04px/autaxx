#ifndef UAI_POSITION_HPP
#define UAI_POSITION_HPP

#include <libataxx/position.hpp>
#include <sstream>

namespace UAI {

// Set the current position
// -- position startpos
// -- position startpos moves a3 d2d4
// -- position fen x5o/7/2-1-2/7/2-1-2/7/o5x x 0 1
// -- position fen x5o/7/2-1-2/7/2-1-2/7/o5x x 0 1 moves a3 d2d4
void position(libataxx::Position &pos, std::stringstream &stream);

}  // namespace UAI

#endif
