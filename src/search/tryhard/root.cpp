#include <array>
#include <cassert>
#include <iostream>
#include "tryhard.hpp"

using namespace std::chrono;

namespace search {

namespace tryhard {

constexpr std::array<int, 4> bounds = {50, 200, 800, 10 * mate_score};

void Tryhard::root(const libataxx::Position pos, const Settings &settings) noexcept {
    const auto t0 = steady_clock::now();

    // Clear
    stats_.clear();
    for (int i = 0; i < max_depth + 1; ++i) {
        stack_[i].ply = i;
        stack_[i].pv.clear();
        stack_[i].killer = libataxx::Move::nomove();
        stack_[i].nullmove = true;
    }

    PV pv;
    const auto start_time = steady_clock::now();
    int depth = max_depth;

    controller_.max_nodes = std::numeric_limits<std::uint64_t>::max();
    controller_.end_time = start_time + hours(1);

    // Time management
    switch (settings.type) {
        case Type::Time: {
            int search_time = 0;

            // Calculate time usage
            if (pos.turn() == libataxx::Side::Black) {
                search_time = settings.btime / 30;
            } else {
                search_time = settings.wtime / 30;
            }

            // Minimum time
            if (search_time < 1) {
                search_time = 1;
            }
            assert(search_time > 0);

            controller_.end_time = start_time + milliseconds(search_time);
            break;
        }
        case Type::Depth:
            depth = settings.depth;
            break;
        case Type::Nodes:
            controller_.max_nodes = settings.nodes;
            break;
        case Type::Movetime:
            controller_.end_time = start_time + milliseconds(settings.movetime);
            break;
        case Type::Infinite:
            break;
        default:
            break;
    }

    // Iterative deepening
    for (int i = 1; i <= depth; ++i) {
        // Aspiration windows
        int score = 0;
        if (i < 3) {
            score = search(stack_, pos, -mate_score, mate_score, i);
        } else {
            int idx_lower = 0;
            int idx_upper = 0;
            while (true) {
                assert(idx_lower < bounds.size());
                assert(idx_upper < bounds.size());
                const int lower = -bounds[idx_lower];
                const int upper = bounds[idx_upper];

                assert(upper > lower);

                score = search(stack_, pos, lower, upper, i);

                if (score <= lower) {
                    idx_lower++;
                } else if (score >= upper) {
                    idx_upper++;
                } else {
                    break;
                }
            }
        }

        const auto finish = steady_clock::now();
        const auto dt = duration_cast<milliseconds>(finish - start_time);

        assert(-mate_score < score && score < mate_score);

        if (i > 1 && (controller_.stop || stats_.nodes >= controller_.max_nodes || finish >= controller_.end_time)) {
            break;
        }

        // Update our main pv
        pv = stack_[0].pv;
        assert(legal_pv(pos, pv));

#ifndef NDEBUG
        // The TT should always have the root position in it
        const auto ttentry = tt_.poll(pos.hash());
        assert(ttentry.hash == pos.hash());
        assert(ttentry.move == pv[0]);
        assert(pos.legal_move(ttentry.move));
        assert(ttentry.depth >= i);
#endif

        // Send info string
        std::cout << "info";
        std::cout << " depth " << i;
        std::cout << " seldepth " << stats_.seldepth;
        std::cout << " score cp " << score;
        std::cout << " time " << dt.count();
        std::cout << " nodes " << stats_.nodes;
        std::cout << " tthits " << stats_.tthits;
        std::cout << " hashfull " << tt_.hashfull();
        if (dt.count() > 0) {
            std::cout << " nps " << 1000 * stats_.nodes / dt.count();
        }
        if (pv.size() > 0) {
            std::cout << " pv";
            for (const auto &move : pv) {
                std::cout << " " << move;
            }
        }
        std::cout << std::endl;
    }

#ifndef NDEBUG
    std::uint64_t total = 0;
    for (int i = 0; i < libataxx::max_moves; ++i) {
        total += stats_.cutoffs[i];
    }
    for (int i = 0; i < 10 && total; ++i) {
        const auto percent = 100 * static_cast<float>(stats_.cutoffs[i]) / total;
        std::cout << "info string";
        std::cout << " index " << i;
        std::cout << " cutoffs " << percent << "%";
        std::cout << std::endl;
    }
#endif

    const auto t1 = steady_clock::now();
    const auto dt = duration_cast<milliseconds>(t1 - t0);
    std::cout << "info time " << dt.count() << "\n";

    if (pv.size() > 0) {
        std::cout << "bestmove " << pv.at(0) << std::endl;
    } else {
        std::cout << "bestmove 0000" << std::endl;
    }
}

}  // namespace tryhard

}  // namespace search
