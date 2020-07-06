#include "alphabeta.hpp"
#include <array>
#include <cassert>
#include <chrono>
#include "phase.hpp"
#include "reduction.hpp"
#include "sorter.hpp"

constexpr std::array<int, 4> futility_margins = {800, 800, 1600, 1600};

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
    } else if (high_resolution_clock::now() >= controller_.end_time) {
        return 0;
    }

    // Update seldepth stats
    stats_.seldepth = std::max(stack->ply, stats_.seldepth);

    // Return mate or draw scores if the game is over
    const auto r = pos.result();
    if (r != libataxx::Result::None) {
        if (r == libataxx::Result::Draw) {
            return 0;
        } else if (r == libataxx::Result::BlackWin) {
            if (pos.turn() == libataxx::Side::Black) {
                return mate_score - stack->ply;
            } else {
                return -mate_score + stack->ply;
            }
        } else if (r == libataxx::Result::WhiteWin) {
            if (pos.turn() == libataxx::Side::White) {
                return mate_score - stack->ply;
            } else {
                return -mate_score + stack->ply;
            }
        } else {
            abort();
        }
    }

    // Make sure we stop searching
    if (depth <= 0 || stack->ply >= max_depth) {
        return eval(pos);
    }

    const bool root = stack->ply == 0;
    const bool pvnode = (beta != alpha + 1);
    const int alpha_orig = alpha;
    libataxx::Move ttmove;

    // Probe transposition table
    const auto ttentry = tt_.poll(pos.hash());
    if (ttentry.hash == pos.hash() && pos.legal_move(ttentry.move)) {
        ttmove = ttentry.move;
        stats_.tthits++;

        if (!pvnode && ttentry.depth >= depth) {
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

    assert(depth > 0);

    // Nullmove pruning
    if (!root && stack->nullmove && depth > 2 && phase(pos) < 0.9) {
        auto npos = pos;
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
    if (!root && stack->nullmove && depth <= futility_margins.size() &&
        static_eval + futility_margins[depth - 1] < alpha) {
        return alpha;
    }

    int best_score = std::numeric_limits<int>::min();
    libataxx::Move best_move;

    // Move generation
    auto sorter = Sorter{pos, ttmove, stack->killer};
    libataxx::Move move;

    // Play every legal move and run negamax on the resulting position
    int i = 0;
    while (sorter.next(move)) {
        stats_.nodes++;
        (stack + 1)->pv.clear();

        auto npos = pos;
        npos.makemove(move);

        int score = 0;
        if (i == 0) {
            score = -alphabeta(stack + 1, npos, -beta, -alpha, depth - 1);
        } else {
            const int r = reduction(npos, i, depth, pvnode);
            score =
                -alphabeta(stack + 1, npos, -alpha - 1, -alpha, depth - 1 - r);
            if (score > alpha) {
                score = -alphabeta(stack + 1, npos, -beta, -alpha, depth - 1);
            }
        }

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
#ifndef NDEBUG
            // Count beta cutoffs
            stats_.cutoffs[i]++;
#endif

            // Killer moves
            stack->killer = move;

            break;
        }

        i++;
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
