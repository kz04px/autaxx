#include <iostream>
#include <libataxx/move.hpp>
#include <libataxx/position.hpp>
#include "../../utils.hpp"
#include "../search.hpp"

namespace search {

namespace random {

void root(const libataxx::Position &pos) {
    libataxx::Move moves[libataxx::max_moves];
    const int num_moves = pos.legal_moves(moves);

    if (num_moves == 0) {
        std::cout << "bestmove 0000" << std::endl;
        return;
    }

    const int idx = utils::rand_u32(0, num_moves - 1);
    std::cout << "bestmove " << moves[idx] << std::endl;
}

}  // namespace random

}  // namespace search
