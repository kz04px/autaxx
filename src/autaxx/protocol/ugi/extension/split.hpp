#ifndef UGI_EXTENSION_SPLIT_HPP
#define UGI_EXTENSION_SPLIT_HPP

#include <libataxx/position.hpp>

namespace UGI::Extension {

// Perform a split perft search
void split(const libataxx::Position &pos, std::stringstream &stream);

}  // namespace UGI::Extension

#endif
