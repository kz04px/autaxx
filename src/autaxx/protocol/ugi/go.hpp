#ifndef UGI_GO_HPP
#define UGI_GO_HPP

#include <libataxx/position.hpp>
#include <sstream>

namespace UGI {

// Stop searching
void stop();

// Start searching for a best move (threaded)
void go(const libataxx::Position &pos, std::stringstream &stream);

}  // namespace UGI

#endif
