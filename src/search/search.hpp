#ifndef SEARCH_SEARCH_HPP
#define SEARCH_SEARCH_HPP

#define MAX_DEPTH 128
#define MATE_SCORE 100000

#include <libataxx/position.hpp>
#include "settings.hpp"

namespace search {

namespace minimax {

void root(const libataxx::Position &pos,
          const search::Settings &options,
          volatile bool *stop);

}  // namespace minimax

namespace random {

void root(const libataxx::Position &pos);

}  // namespace random

}  // namespace search

#endif
