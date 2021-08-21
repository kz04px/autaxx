#ifndef UAI_MOVES_HPP
#define UAI_MOVES_HPP

#include <libataxx/position.hpp>
#include <sstream>

namespace UAI {

// Apply a series of moves to the position
// -- moves a3 d2d4
void moves(libataxx::Position &pos, std::stringstream &stream);

}  // namespace UAI

#endif
