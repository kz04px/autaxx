#ifndef UAI_EXTENSION_SPLIT_HPP
#define UAI_EXTENSION_SPLIT_HPP

#include <libataxx/position.hpp>

namespace UAI {

namespace Extension {

// Perform a split perft search
void split(const libataxx::Position &pos, std::stringstream &stream);

}  // namespace Extension

}  // namespace UAI

#endif
