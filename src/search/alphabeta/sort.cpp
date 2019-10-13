#include <cassert>
#include "alphabeta.hpp"

namespace search {

namespace alphabeta {

void Alphabeta::sort(const libataxx::Position &pos,
                     libataxx::Move *moves,
                     const int num_moves) noexcept {
    assert(moves);

    int scores[libataxx::max_moves] = {};

    // Score
    for (int i = 0; i < num_moves; ++i) {
        scores[i] = pos.count_captures(moves[i]);
        scores[i] += (moves[i].is_single() ? 1 : 0);
    }

    // Sort
    for (int i = 0; i < num_moves - i; ++i) {
        int idx = i;
        for (int j = i + 1; j < num_moves; ++j) {
            if (scores[j] > scores[idx]) {
                idx = j;
            }
        }
        std::swap(moves[i], moves[idx]);
        std::swap(scores[i], scores[idx]);
    }
}

}  // namespace alphabeta

}  // namespace search
