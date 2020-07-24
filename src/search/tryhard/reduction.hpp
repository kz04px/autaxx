#ifndef SEARCH_TRYHARD_REDUCTION_HPP
#define SEARCH_TRYHARD_REDUCTION_HPP

#include <cassert>
#include <libataxx/position.hpp>

namespace search {

namespace tryhard {

int reduction(const libataxx::Position &npos, const int move_num, const int depth, const bool pvnode) {
    assert(depth >= 0);
    assert(move_num >= 0);

    if (move_num < 3) {
        return 0;
    } else if (move_num < 16) {
        return 2;
    }

    return 4;
}

}  // namespace tryhard

}  // namespace search

#endif
