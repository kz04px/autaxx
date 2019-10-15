#include "alphabeta.hpp"
#include <cassert>
#include <chrono>
#include "phase.hpp"
#include "sorter.hpp"

using namespace std::chrono;

namespace search {

namespace alphabeta {

int Alphabeta::alphabeta(Stack *stack,
                         const libataxx::Position &pos,
                         int alpha,
                         const int beta,
                         int depth) {
    assert(stack);
    assert(alpha < beta);

    // Stop if asked
    if (controller_.stop) {
        return 0;
    } else if (stats_.nodes >= controller_.max_nodes) {
        return 0;
    } else if (high_resolution_clock::now() > controller_.end_time) {
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

    // Nullmove pruning
    if (stack->ply > 0 && stack->nullmove && depth > 2 && phase(pos) < 0.9) {
        libataxx::Position npos = pos;
        npos.makemove(libataxx::Move::nullmove());

        (stack + 1)->nullmove = false;
        const int score =
            -alphabeta(stack + 1, npos, -beta, -beta + 1, depth - 3);
        (stack + 1)->nullmove = true;

        if (score >= beta) {
            return score;
        }
    }

    int best_score = std::numeric_limits<int>::min();

    // Move generation
    auto sorter = Sorter{pos};
    libataxx::Move move;

    // Play every legal move and run negamax on the resulting position
    while (sorter.next(move)) {
        stats_.nodes++;
        (stack + 1)->pv.clear();

        libataxx::Position npos = pos;
        npos.makemove(move);
        const int score = -alphabeta(stack + 1, npos, -beta, -alpha, depth - 1);

        if (score > best_score) {
            alpha = score;

            // Update PV
            stack->pv.clear();
            stack->pv.push_back(move);
            stack->pv.insert(stack->pv.begin() + 1,
                             (stack + 1)->pv.begin(),
                             (stack + 1)->pv.end());

            best_score = score;
        }

        if (alpha >= beta) {
            break;
        }
    }

    return best_score;
}

}  // namespace alphabeta

}  // namespace search
