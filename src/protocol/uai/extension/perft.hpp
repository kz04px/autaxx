#ifndef UAI_EXTENSION_PERFT_HPP
#define UAI_EXTENSION_PERFT_HPP

#include <libataxx/position.hpp>
#include <sstream>

namespace UAI::Extension {

// Perform a perft search
void perft(const libataxx::Position &pos, std::stringstream &stream);

}  // namespace UAI::Extension

#endif
