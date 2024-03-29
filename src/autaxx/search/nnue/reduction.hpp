#ifndef SEARCH_NNUE_REDUCTION_HPP
#define SEARCH_NNUE_REDUCTION_HPP

#include <cassert>
#include <libataxx/position.hpp>

namespace search::nnue {

[[nodiscard]] int reduction(const libataxx::Position &,
                            const int move_num,
                            [[maybe_unused]] const int depth,
                            [[maybe_unused]] const bool pvnode) {
    assert(depth >= 0);
    assert(move_num >= 0);

    if (move_num < 9) {
        return 2;
    }

    return 3;
}

}  // namespace search::nnue

#endif
