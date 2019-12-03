#ifndef SEARCH_ALPHABETA_REDUCTION_HPP
#define SEARCH_ALPHABETA_REDUCTION_HPP

#include <libataxx/position.hpp>

namespace search {

namespace alphabeta {

int reduction(const libataxx::Position &npos,
              const int move_num,
              const int depth,
              const bool pvnode) {
    return move_num < 3 ? 0 : 2;
}

}  // namespace alphabeta

}  // namespace search

#endif
