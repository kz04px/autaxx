#ifndef ALPHABETA_EVAL_HPP
#define ALPHABETA_EVAL_HPP

#include <libataxx/position.hpp>

namespace search {

namespace alphabeta {

// Return the evaluation of the position from the side to move's point of view
int eval(const libataxx::Position &pos) {
    const int num_us = pos.us().count();
    const int num_them = pos.them().count();
    return 100 * (num_us - num_them);
}

}  // namespace alphabeta

}  // namespace search

#endif
