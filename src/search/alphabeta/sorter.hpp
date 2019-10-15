#ifndef SEARCH_ALPHABETA_SORTER_HPP
#define SEARCH_ALPHABETA_SORTER_HPP

#include <libataxx/move.hpp>
#include <libataxx/position.hpp>

namespace search {

namespace alphabeta {

class Sorter {
   public:
    Sorter(const libataxx::Position &pos, const libataxx::Move &killer) {
        num_moves_ = pos.legal_moves(moves_);
        score(pos, killer);
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
               const libataxx::Move &killer) noexcept {
        for (int i = 0; i < num_moves_; ++i) {
            if (moves_[i] == killer) {
                scores_[i] = 100;
            } else {
                scores_[i] = pos.count_captures(moves_[i]);
                scores_[i] += (moves_[i].is_single() ? 1 : 0);
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
