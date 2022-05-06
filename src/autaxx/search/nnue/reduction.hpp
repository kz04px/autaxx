#ifndef SEARCH_NNUE_REDUCTION_HPP
#define SEARCH_NNUE_REDUCTION_HPP

#include <cassert>
#include <libataxx/position.hpp>

namespace search::nnue {

[[nodiscard]] int reduction(const libataxx::Position &npos, const int move_num, const int depth, const bool pvnode) {
    assert(depth >= 0);
    assert(move_num >= 0);

    if (move_num < 3) {
        return 0;
    } else if (move_num < 16) {
        return 2;
    }

    return 4;
}

}  // namespace search::nnue

#endif
