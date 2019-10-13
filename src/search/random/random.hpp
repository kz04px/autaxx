#ifndef SEARCH_RANDOM_HPP
#define SEARCH_RANDOM_HPP

#include <iostream>
#include <libataxx/move.hpp>
#include <libataxx/position.hpp>
#include "../../utils.hpp"
#include "../search.hpp"

namespace search {

namespace random {

class Random : public Search {
   public:
    void go(const libataxx::Position pos, const Settings &settings) override {
        libataxx::Move moves[libataxx::max_moves];
        const int num_moves = pos.legal_moves(moves);

        if (num_moves == 0) {
            std::cout << libataxx::Move::nullmove() << std::endl;
            return;
        }

        const int idx = utils::rand_u32(0, num_moves - 1);
        std::cout << "bestmove " << moves[idx] << std::endl;
    }

   private:
};

}  // namespace random

}  // namespace search

#endif
