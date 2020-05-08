#ifndef SEARCH_MCTS_EVAL_HPP
#define SEARCH_MCTS_EVAL_HPP

#include <libataxx/position.hpp>

namespace search::mcts {

int eval(const libataxx::Position &pos);

}  // namespace search::mcts

#endif
