#ifndef UGI_EXTENSION_PERFT_HPP
#define UGI_EXTENSION_PERFT_HPP

#include <libataxx/position.hpp>
#include <sstream>

namespace UGI::Extension {

// Perform a perft search
void perft(const libataxx::Position &pos, std::stringstream &stream);

}  // namespace UGI::Extension

#endif
