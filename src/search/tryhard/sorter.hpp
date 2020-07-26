#ifndef SEARCH_TRYHARD_SORTER_HPP
#define SEARCH_TRYHARD_SORTER_HPP

#include <libataxx/move.hpp>
#include <libataxx/position.hpp>

namespace search {

namespace tryhard {

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

enum class Stage : std::uint8_t
{
    TT = 0,
    Killer,
    Captures,
    Noncaptures,
    Done
};

class Sorter {
   public:
    Sorter(const libataxx::Position &pos, const libataxx::Move &ttmove, const libataxx::Move &killer)
        : pos_{pos}, ttmove_{ttmove}, killer_{killer}, num_moves_{0}, moves_{}, stage_{Stage::TT} {
    }

    [[nodiscard]] bool next(libataxx::Move &move) noexcept {
        if (num_moves_ == 0) {
            next_stage();
            if (num_moves_ == 0) {
                return false;
            }
        }

        int idx = 0;
        for (int i = 1; i < num_moves_; ++i) {
            if (scores_[i] > scores_[idx]) {
                idx = i;
            }
        }

        move = moves_[idx];

        moves_[idx] = moves_[num_moves_ - 1];
        scores_[idx] = scores_[num_moves_ - 1];
        num_moves_--;

        return true;
    }

   private:
    void next_stage() noexcept {
        if (stage_ == Stage::TT) {
            stage_ = Stage::Killer;

            if (ttmove_ && pos_.legal_move(ttmove_)) {
                moves_[0] = ttmove_;
                num_moves_ = 1;
                return;
            }
        }

        if (stage_ == Stage::Killer) {
            stage_ = Stage::Captures;

            if (killer_ && killer_ != ttmove_ && pos_.legal_move(killer_)) {
                moves_[0] = killer_;
                num_moves_ = 1;
                return;
            }
        }

        if (stage_ == Stage::Captures) {
            stage_ = Stage::Noncaptures;

            num_moves_ = pos_.legal_captures(moves_);

            // Scores
            for (int i = num_moves_ - 1; i >= 0; --i) {
                assert(moves_[i] != libataxx::Move::nullmove());
                assert(moves_[i] != libataxx::Move::nomove());
                assert(pos_.count_captures(moves_[i]));

                if (moves_[i] == ttmove_ || moves_[i] == killer_) {
                    moves_[i] = moves_[num_moves_ - 1];
                    scores_[i] = scores_[num_moves_ - 1];
                    num_moves_--;
                    continue;
                }

                const auto captures = pos_.count_captures(moves_[i]);
                scores_[i] = 100 * captures + (moves_[i].is_single() ? 100 : 0);
            }

            if (num_moves_ > 0) {
                return;
            }
        }

        if (stage_ == Stage::Noncaptures) {
            stage_ = Stage::Done;

            num_moves_ = pos_.legal_noncaptures(moves_);

            if (num_moves_ == 1 && moves_[0] == libataxx::Move::nullmove()) {
                if (killer_ == libataxx::Move::nullmove() || ttmove_ == libataxx::Move::nullmove()) {
                    num_moves_ = 0;
                }
                return;
            }

            // Scores
            for (int i = num_moves_ - 1; i >= 0; --i) {
                assert(moves_[i] != libataxx::Move::nullmove());
                assert(moves_[i] != libataxx::Move::nomove());
                assert(moves_[i].to().index() < 49);
                assert(moves_[i].from().index() < 49);
                assert(!pos_.count_captures(moves_[i]));

                if (moves_[i] == ttmove_ || moves_[i] == killer_) {
                    moves_[i] = moves_[num_moves_ - 1];
                    scores_[i] = scores_[num_moves_ - 1];
                    num_moves_--;
                    continue;
                }

                if (moves_[i].is_single()) {
                    scores_[i] = pst[moves_[i].to().index()];
                } else {
                    scores_[i] = pst[moves_[i].to().index()] - pst[moves_[i].from().index()];
                }
            }

            if (num_moves_ > 0) {
                return;
            }
        }
    }

   private:
    libataxx::Position pos_;
    libataxx::Move ttmove_;
    libataxx::Move killer_;
    int num_moves_;
    libataxx::Move moves_[libataxx::max_moves];
    int scores_[libataxx::max_moves];
    Stage stage_;
};

}  // namespace tryhard

}  // namespace search

#endif
