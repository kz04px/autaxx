#ifndef SEARCH_LEASTCAPTURES_HPP
#define SEARCH_LEASTCAPTURES_HPP

#include <climits>
#include <iostream>
#include <libataxx/move.hpp>
#include <libataxx/position.hpp>
#include "../../utils.hpp"
#include "../search.hpp"

namespace search {

namespace leastcaptures {

class LeastCaptures : public Search {
   public:
    void go(const libataxx::Position pos, const Settings &settings) override {
        libataxx::Move moves[libataxx::max_moves];
        const int num_moves = pos.legal_moves(moves);

        if (num_moves == 0 || moves[0] == libataxx::Move::nullmove()) {
            std::cout << "bestmove 0000" << std::endl;
            return;
        }

        int least_captures = std::numeric_limits<int>::max();
        std::vector<libataxx::Move> best_moves;

        for (int i = 0; i < num_moves; ++i) {
            int num_captures = pos.count_captures(moves[i]);

            if (moves[i].is_single()) {
                num_captures++;
            }

            if (num_captures < least_captures) {
                best_moves.clear();
                best_moves.push_back(moves[i]);
                least_captures = num_captures;
            } else if (num_captures == least_captures) {
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

}  // namespace leastcaptures

}  // namespace search

#endif
