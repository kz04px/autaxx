#ifndef UAI_EXTENSION_SPLIT_HPP
#define UAI_EXTENSION_SPLIT_HPP

#include <libataxx/position.hpp>

namespace UAI::Extension {

// Perform a split perft search
void split(const libataxx::Position &pos, std::stringstream &stream);

}  // namespace UAI::Extension

#endif
