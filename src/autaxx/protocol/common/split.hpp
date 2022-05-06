#ifndef COMMON_SPLIT_HPP
#define COMMON_SPLIT_HPP

#include <libataxx/position.hpp>

namespace common {

// Perform a split perft search
void split(const libataxx::Position &pos, std::stringstream &stream);

}  // namespace common

#endif
