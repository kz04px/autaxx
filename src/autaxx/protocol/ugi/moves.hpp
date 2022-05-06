#ifndef UGI_MOVES_HPP
#define UGI_MOVES_HPP

#include <libataxx/position.hpp>
#include <sstream>

namespace UGI {

// Apply a series of moves to the position
// -- moves a3 d2d4
void moves(libataxx::Position &pos, std::stringstream &stream);

}  // namespace UGI

#endif
