#ifndef SEARCH_ALPHABETA_SORTER_HPP
#define SEARCH_ALPHABETA_SORTER_HPP

#include <libataxx/move.hpp>
#include <libataxx/position.hpp>

namespace search {

namespace alphabeta {

// clang-format off
constexpr int pst[49] = {
    30, 20, 10, 10, 10, 20, 30,
    20, 10, 10,  5, 10, 10, 20,
    10, 10,  5,  0,  5, 10, 10,
    10,  5,  0,  0,  0,  5, 10,
    10, 10,  5,  0,  5, 10, 10,
    20, 10, 10,  5, 10, 10, 20,
    30, 20, 10, 10, 10, 20, 30,
};
// clang-format on

class Sorter {
   public:
    Sorter(const libataxx::Position &pos,
           const libataxx::Move &ttmove,
           const libataxx::Move &killer) {
        num_moves_ = pos.legal_moves(moves_);
        score(pos, ttmove, killer);
    }

    [[nodiscard]] bool next(libataxx::Move &move) noexcept {
        if (num_moves_ == 0) {
            return false;
        }

        int idx = 0;
        for (int i = 0; i < num_moves_; ++i) {
            if (scores_[i] > scores_[idx]) {
                idx = i;
            }
        }

        move = moves_[idx];

        scores_[idx] = scores_[num_moves_ - 1];
        moves_[idx] = moves_[num_moves_ - 1];
        num_moves_--;

        return true;
    }

   private:
    void score(const libataxx::Position &pos,
               const libataxx::Move &ttmove,
               const libataxx::Move &killer) noexcept {
        if (num_moves_ <= 1) {
            return;
        }

        for (int i = 0; i < num_moves_; ++i) {
            if (moves_[i] == ttmove) {
                scores_[i] = 10000;
                continue;
            } else if (moves_[i] == killer) {
                scores_[i] = 9999;
                continue;
            }

            const auto captures = pos.count_captures(moves_[i]);

            if (captures) {
                scores_[i] = 100 * captures;
                scores_[i] += moves_[i].is_single() ? 100 : 0;
            } else {
                if (moves_[i].is_single()) {
                    scores_[i] = pst[static_cast<int>(moves_[i].to())];
                } else {
                    scores_[i] = pst[static_cast<int>(moves_[i].to())] -
                                 pst[static_cast<int>(moves_[i].from())];
                }
            }
        }
    }

    int num_moves_;
    libataxx::Move moves_[libataxx::max_moves];
    int scores_[libataxx::max_moves];
};

}  // namespace alphabeta

}  // namespace search

#endif
