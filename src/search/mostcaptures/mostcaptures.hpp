#ifndef SEARCH_MOSTCAPTURES_HPP
#define SEARCH_MOSTCAPTURES_HPP

#include <iostream>
#include <libataxx/move.hpp>
#include <libataxx/position.hpp>
#include "../../utils.hpp"
#include "../search.hpp"

namespace search {

namespace mostcaptures {

class MostCaptures : public Search {
   public:
    void go(const libataxx::Position pos, const Settings &settings) override {
        libataxx::Move moves[libataxx::max_moves];
        const int num_moves = pos.legal_moves(moves);

        if (num_moves == 0 || moves[0] == libataxx::Move::nullmove()) {
            std::cout << "bestmove 0000" << std::endl;
            return;
        }

        int most_captures = 0;
        std::vector<libataxx::Move> best_moves;

        for (int i = 0; i < num_moves; ++i) {
            int num_captures = pos.count_captures(moves[i]);

            if (moves[i].is_single()) {
                num_captures++;
            }

            if (num_captures > most_captures) {
                best_moves.clear();
                best_moves.push_back(moves[i]);
                most_captures = num_captures;
            } else if (num_captures == most_captures) {
                best_moves.push_back(moves[i]);
            }
        }

        const int idx = utils::rand_u32(0, best_moves.size() - 1);

        // Material only eval
        auto npos = pos;
        npos.makemove(best_moves[idx]);
        const auto score = -100 * (npos.us().count() - npos.them().count());
        std::cout << "info";
        std::cout << " score cp " << score;
        std::cout << std::endl;

        std::cout << "bestmove " << best_moves[idx] << std::endl;
    }

   private:
};

}  // namespace mostcaptures

}  // namespace search

#endif
