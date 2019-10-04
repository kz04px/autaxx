#ifndef PLAYERS_MINIMAX_HPP
#define PLAYERS_MINIMAX_HPP

#include <libataxx/position.hpp>
#include "../controller.hpp"
#include "../stack.hpp"
#include "../stats.hpp"

namespace search {

namespace minimax {

int minimax(Controller &controller,
            Stats &stats,
            Stack *stack,
            const libataxx::Position &pos,
            const int depth);

}  // namespace minimax

}  // namespace search

#endif
