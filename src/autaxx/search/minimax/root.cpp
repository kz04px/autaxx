#include <cassert>
#include <iostream>
#include "minimax.hpp"

using namespace std::chrono;

namespace search::minimax {

void Minimax::root(const libataxx::Position pos, const Settings &settings) noexcept {
    // Clear
    stats_.clear();
    for (int i = 0; i < max_depth + 1; ++i) {
        stack_[i].ply = i;
        stack_[i].pv.clear();
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
            if (pos.get_turn() == libataxx::Side::Black) {
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
        const int score = minimax(stack_, pos, i);
        const auto finish = steady_clock::now();

        assert(-mate_score < score && score < mate_score);

        if (i > 1 && (controller_.stop || stats_.nodes >= controller_.max_nodes ||
                      steady_clock::now() >= controller_.end_time)) {
            break;
        }

        // Update our main pv
        pv = stack_[0].pv;
        assert(legal_pv(pos, pv));

        // Send info string
        const auto dt = duration_cast<milliseconds>(finish - start_time);
        std::cout << "info";
        std::cout << " depth " << i;
        std::cout << " seldepth " << stats_.seldepth;
        std::cout << " score cp " << score;
        std::cout << " time " << dt.count();
        std::cout << " nodes " << stats_.nodes;
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

    if (pv.size() > 0) {
        std::cout << "bestmove " << pv.at(0) << std::endl;
    } else {
        std::cout << "bestmove 0000" << std::endl;
    }
}

}  // namespace search::minimax
