#include <cassert>
#include <libataxx/position.hpp>
#include <limits>
#include "eval.hpp"
#include "search.hpp"

// Minimax algorithm (negamax)
int minimax(SearchController &controller,
            SearchStats &stats,
            SearchStack *stack,
            const libataxx::Position &pos,
            const int depth) {
    assert(stack);
    assert(controller.stop);

    // Stop if asked
    if (*controller.stop) {
        return 0;
    } else if (stats.nodes >= controller.max_nodes) {
        return 0;
    } else if (std::chrono::high_resolution_clock::now() >
               controller.end_time) {
        return 0;
    }

    // Update seldepth stats
    stats.seldepth = std::max(stack->ply, stats.seldepth);

    // Return mate or draw scores if the game is over
    if (pos.gameover()) {
        const int num_us = pos.us().count();
        const int num_them = pos.them().count();

        if (num_us > num_them) {
            return MATE_SCORE - stack->ply;
        } else if (num_us < num_them) {
            return -MATE_SCORE + stack->ply;
        } else {
            return 0;
        }
    }

    // Make sure we stop searching
    if (depth == 0 || stack->ply >= MAX_DEPTH) {
        return eval(pos);
    }

    int best_score = std::numeric_limits<int>::min();

    // Move generation
    libataxx::Move moves[libataxx::max_moves];
    const int num_moves = pos.legal_moves(moves);

    // Keeping track of the node count
    stats.nodes += num_moves;

    // Play every legal move and run negamax on the resulting position
    for (int i = 0; i < num_moves; ++i) {
        (stack + 1)->pv.clear();

        libataxx::Position npos = pos;
        npos.makemove(moves[i]);
        const int score =
            -minimax(controller, stats, stack + 1, npos, depth - 1);

        if (score > best_score) {
            // Update PV
            stack->pv.clear();
            stack->pv.push_back(moves[i]);
            stack->pv.insert(stack->pv.begin() + 1,
                             (stack + 1)->pv.begin(),
                             (stack + 1)->pv.end());

            best_score = score;
        }
    }

    return best_score;
}
