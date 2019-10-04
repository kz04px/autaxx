#ifndef MINIMAX_EVAL_HPP
#define MINIMAX_EVAL_HPP

#include <libataxx/position.hpp>

namespace search {

namespace minimax {

// Return the evaluation of the position from the side to move's point of view
int eval(const libataxx::Position &pos) {
    const int num_us = pos.us().count();
    const int num_them = pos.them().count();
    return 100 * (num_us - num_them);
}

}  // namespace minimax

}  // namespace search

#endif
