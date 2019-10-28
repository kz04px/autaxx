#include "alphabeta.hpp"
#include <cassert>
#include <chrono>
#include "phase.hpp"
#include "sorter.hpp"

constexpr int futility_margins[] = {800, 800, 1600, 1600};

using namespace std::chrono;

namespace search {

namespace alphabeta {

int Alphabeta::alphabeta(Stack *stack,
                         const libataxx::Position &pos,
                         int alpha,
                         int beta,
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

    const int alpha_orig = alpha;
    libataxx::Move ttmove;

    // Probe transposition table
    const auto ttentry = tt_.poll(pos.hash());
    if (ttentry.hash == pos.hash() && pos.legal_move(ttentry.move)) {
        ttmove = ttentry.move;
        stats_.tthits++;

        if (ttentry.depth >= depth) {
            const int entry_score = eval_from_tt(ttentry.score, stack->ply);

            switch (ttentry.flag) {
                case TTEntry::Flag::Exact:
                    // Update PV
                    stack->pv.clear();
                    stack->pv.push_back(ttentry.move);
                    return entry_score;
                case TTEntry::Flag::Lower:
                    alpha = std::max(alpha, entry_score);
                    break;
                case TTEntry::Flag::Upper:
                    beta = std::min(beta, entry_score);
                    break;
                default:
                    abort();
                    break;
            }

            if (alpha >= beta) {
                // Update PV
                stack->pv.clear();
                stack->pv.push_back(ttentry.move);
                return entry_score;
            }
        }
    }

    const auto static_eval = eval(pos);
    const bool root = stack->ply == 0;

    assert(depth > 0);

    // Nullmove pruning
    if (!root && stack->nullmove && depth > 2 && phase(pos) < 0.9) {
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

    // Reverse futility pruning
    if (!root && stack->nullmove && depth <= std::size(futility_margins) &&
        static_eval + futility_margins[depth - 1] < alpha) {
        return alpha;
    }

    int best_score = std::numeric_limits<int>::min();
    libataxx::Move best_move;

    // Move generation
    auto sorter = Sorter{pos, ttmove, stack->killer};
    libataxx::Move move;

    // Play every legal move and run negamax on the resulting position
    while (sorter.next(move)) {
        stats_.nodes++;
        (stack + 1)->pv.clear();

        libataxx::Position npos = pos;
        npos.makemove(move);

        const int score = -alphabeta(stack + 1, npos, -beta, -alpha, depth - 1);

        if (score > best_score) {
            best_score = score;
            best_move = move;
            // Update PV
            stack->pv.clear();
            stack->pv.push_back(move);
            stack->pv.insert(stack->pv.begin() + 1,
                             (stack + 1)->pv.begin(),
                             (stack + 1)->pv.end());

            if (score > alpha) {
                alpha = score;
            }
        }

        if (alpha >= beta) {
            if (pos.count_captures(move) + (move.is_single() ? 1 : 0) <= 1) {
                stack->killer = move;
            }
            break;
        }
    }

    // Add to transposition table
    TTEntry nentry;
    nentry.hash = pos.hash();
    nentry.move = best_move;
    nentry.score = eval_to_tt(best_score, stack->ply);
    nentry.depth = depth;
    nentry.flag = TTEntry::Flag::Exact;
    if (best_score <= alpha_orig) {
        nentry.flag = TTEntry::Flag::Upper;
    } else if (best_score >= beta) {
        nentry.flag = TTEntry::Flag::Lower;
    }
    tt_.add(pos.hash(), nentry);

    assert(tt_.poll(pos.hash()) == nentry);

    return alpha;
}

}  // namespace alphabeta

}  // namespace search
