#ifndef PLAYERS_MINIMAX_HPP
#define PLAYERS_MINIMAX_HPP

#include <libataxx/position.hpp>
#include "../controller.hpp"
#include "../stack.hpp"
#include "../stats.hpp"

namespace search {

namespace alphabeta {

int alphabeta(Controller &controller,
              Stats &stats,
              Stack *stack,
              const libataxx::Position &pos,
              int alpha,
              const int beta,
              int depth);

}  // namespace alphabeta

}  // namespace search

#endif
