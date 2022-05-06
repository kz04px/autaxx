#ifndef COMMON_PERFT_HPP
#define COMMON_PERFT_HPP

#include <libataxx/position.hpp>
#include <sstream>

namespace common {

// Perform a perft search
void perft(const libataxx::Position &pos, std::stringstream &stream);

}  // namespace common

#endif
