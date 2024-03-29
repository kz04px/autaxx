#include <cassert>
#include <chrono>
#include "alphabeta.hpp"

using namespace std::chrono;

namespace search::alphabeta {

int Alphabeta::search(Stack *stack, const libataxx::Position &pos, int alpha, const int beta, int depth) {
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
    const auto num_us = pos.get_us().count();
    const auto num_them = pos.get_them().count();
    switch (pos.get_result()) {
        case libataxx::Result::None:
            break;
        case libataxx::Result::BlackWin:
        case libataxx::Result::WhiteWin:
            return num_us > num_them ? mate_score - stack->ply : num_them > num_us ? -mate_score + stack->ply : 0;
        case libataxx::Result::Draw:
            return 0;
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

    // Play every legal move and run negamax on the resulting position
    for (int i = 0; i < num_moves; ++i) {
        stats_.nodes++;
        (stack + 1)->pv.clear();

        auto npos = pos;
        npos.makemove(moves[i]);
        const int score = -search(stack + 1, npos, -beta, -alpha, depth - 1);

        if (score > best_score) {
            // Update PV
            stack->pv.clear();
            stack->pv.push_back(moves[i]);
            stack->pv.insert(stack->pv.begin() + 1, (stack + 1)->pv.begin(), (stack + 1)->pv.end());

            best_score = score;

            if (score > alpha) {
                alpha = score;
            }

            if (alpha >= beta) {
                break;
            }
        }
    }

    return best_score;
}

}  // namespace search::alphabeta
