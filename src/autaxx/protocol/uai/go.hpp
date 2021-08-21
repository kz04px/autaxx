#ifndef UAI_GO_HPP
#define UAI_GO_HPP

#include <libataxx/position.hpp>
#include <sstream>

namespace UAI {

// Stop searching
void stop();

// Start searching for a best move (threaded)
void go(const libataxx::Position &pos, std::stringstream &stream);

}  // namespace UAI

#endif
