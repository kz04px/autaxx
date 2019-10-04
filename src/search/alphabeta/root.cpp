#include <chrono>
#include <iostream>
#include "../../options.hpp"
#include "../controller.hpp"
#include "../pv.hpp"
#include "../search.hpp"
#include "../settings.hpp"
#include "../stack.hpp"
#include "../stats.hpp"
#include "alphabeta.hpp"

using namespace std::chrono;

namespace search {

namespace alphabeta {

// Perform a search as specified in the options
void root(const libataxx::Position &pos,
          const search::Settings &options,
          volatile bool *stop) {
    assert(stop);

    int depth = MAX_DEPTH;
    const auto start_time = high_resolution_clock::now();
    PV pv;
    Stats stats;
    Stack stack[MAX_DEPTH + 1];
    Controller controller;
    controller.stop = stop;
    controller.max_nodes = std::numeric_limits<std::uint64_t>::max();
    controller.end_time = start_time + hours(1);

    switch (options.type) {
        case search::Type::Time: {
            int search_time = 0;

            // Calculate time usage
            if (pos.turn() == libataxx::Side::Black) {
                search_time = options.btime / 30;
            } else {
                search_time = options.wtime / 30;
            }

            // Minimum time
            if (search_time < 1) {
                search_time = 1;
            }
            assert(search_time > 0);

            controller.end_time = start_time + milliseconds(search_time);
            break;
        }
        case search::Type::Depth:
            depth = options.depth;
            break;
        case search::Type::Nodes:
            controller.max_nodes = options.nodes;
            break;
        case search::Type::Movetime:
            controller.end_time = start_time + milliseconds(options.movetime);
            break;
        case search::Type::Infinite:
            break;
        default:
            break;
    }

    // Set stack
    for (int i = 0; i < MAX_DEPTH + 1; ++i) {
        stack[i].ply = i;
    }

    // Iterative deepening
    for (int i = 1; i <= depth; ++i) {
        const int score = alphabeta(
            controller, stats, stack, pos, -MATE_SCORE, MATE_SCORE, i);
        auto finish = high_resolution_clock::now();

        assert(-MATE_SCORE < score && score < MATE_SCORE);

        if (i > 1 && (*stop || stats.nodes >= controller.max_nodes ||
                      high_resolution_clock::now() > controller.end_time)) {
            break;
        }

        // Update our main pv
        pv = stack[0].pv;
        assert(legal_pv(pos, pv));

        // Send info string
        duration<double> elapsed = finish - start_time;
        std::cout << "info"
                  << " score cs " << score << " depth " << i << " seldepth "
                  << stats.seldepth << " time "
                  << static_cast<int>(elapsed.count() * 1000) << " nodes "
                  << stats.nodes;
        if (elapsed.count() > 0) {
            std::cout << " nps "
                      << static_cast<std::uint64_t>(stats.nodes /
                                                    elapsed.count());
        }
        if (pv.size() > 0) {
            std::cout << " pv";
            for (const auto &move : pv) {
                std::cout << " " << move;
            }
        }
        std::cout << std::endl;
    }

    // Send our best move
    if (Options::checks["Ponder"].get() && pv.size() >= 2) {
        std::cout << "bestmove " << pv.at(0) << "ponder " << pv.at(1)
                  << std::endl;
    } else if (pv.size() >= 1) {
        std::cout << "bestmove " << pv.at(0) << std::endl;
    } else {
        std::cout << "bestmove 0000" << std::endl;
    }
}

}  // namespace alphabeta

}  // namespace search
