#include "minimax.hpp"
#include <cassert>
#include <chrono>

using namespace std::chrono;

namespace search::minimax {

[[nodiscard]] int Minimax::minimax(Stack *stack, const libataxx::Position &pos, int depth) {
    assert(stack);

    // Stop if asked
    if (controller_.stop) {
        return 0;
    } else if (stats_.nodes >= controller_.max_nodes) {
        return 0;
    } else if (steady_clock::now() > controller_.end_time) {
        return 0;
    }

    // Update seldepth stats
    stats_.seldepth = std::max(stack->ply, stats_.seldepth);

    // Return mate or draw scores if the game is over
    if (pos.gameover()) {
        const int num_us = pos.us().count();
        const int num_them = pos.them().count();

        if (num_us > num_them) {
            return mate_score - stack->ply;
        } else if (num_us < num_them) {
            return -mate_score + stack->ply;
        } else {
            return 0;
        }
    }

    // Make sure we stop searching
    if (depth == 0 || stack->ply >= max_depth) {
        return eval(pos);
    }

    int best_score = std::numeric_limits<int>::min();

    // Move generation
    libataxx::Move moves[libataxx::max_moves];
    const int num_moves = pos.legal_moves(moves);

    assert(num_moves > 0);

    // Keeping track of the node count
    stats_.nodes += num_moves;

    // Play every legal move and run negamax on the resulting position
    for (int i = 0; i < num_moves; ++i) {
        (stack + 1)->pv.clear();

        auto npos = pos;
        npos.makemove(moves[i]);
        const int score = -minimax(stack + 1, npos, depth - 1);

        if (score > best_score) {
            // Update PV
            stack->pv.clear();
            stack->pv.push_back(moves[i]);
            stack->pv.insert(stack->pv.begin() + 1, (stack + 1)->pv.begin(), (stack + 1)->pv.end());

            best_score = score;
        }
    }

    return best_score;
}

}  // namespace search::minimax
