#include "search.hpp"
#include <chrono>
#include <iostream>
#include "options.hpp"

// Perform a search as specified in the options
void search(const libataxx::Position &pos,
            const SearchOptions &options,
            volatile bool *stop) {
    assert(stop);

    int depth = MAX_DEPTH;
    const auto start_time = std::chrono::high_resolution_clock::now();
    PV pv;
    SearchStats stats;
    SearchStack stack[MAX_DEPTH + 1];
    SearchController controller;
    controller.stop = stop;
    controller.max_nodes = std::numeric_limits<std::uint64_t>::max();
    controller.end_time = start_time + std::chrono::hours(1);

    switch (options.type) {
        case SearchType::Time: {
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

            controller.end_time =
                start_time + std::chrono::milliseconds(search_time);
            break;
        }
        case SearchType::Depth:
            depth = options.depth;
            break;
        case SearchType::Nodes:
            controller.max_nodes = options.nodes;
            break;
        case SearchType::Movetime:
            controller.end_time =
                start_time + std::chrono::milliseconds(options.movetime);
            break;
        case SearchType::Infinite:
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
        const int score = minimax(controller, stats, stack, pos, i);
        auto finish = std::chrono::high_resolution_clock::now();

        assert(-MATE_SCORE < score && score < MATE_SCORE);

        if (i > 1 &&
            (*stop || stats.nodes >= controller.max_nodes ||
             std::chrono::high_resolution_clock::now() > controller.end_time)) {
            break;
        }

        // Update our main pv
        pv = stack[0].pv;
        assert(legal_pv(pos, pv));

        // Send info string
        std::chrono::duration<double> elapsed = finish - start_time;
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

// Check the legality of a PV based on a given board
bool legal_pv(const libataxx::Position &pos, const PV &pv) {
    libataxx::Position npos = pos;
    for (const auto &move : pv) {
        if (!npos.legal_move(move)) {
            return false;
        }
        npos.makemove(move);
    }
    return true;
}
