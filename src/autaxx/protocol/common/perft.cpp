#include "perft.hpp"
#include <chrono>
#include <iostream>

namespace common {

// Perform a perft search
void perft(const libataxx::Position &pos, std::stringstream &stream) {
    int depth = 0;
    stream >> depth;
    if (depth < 1) {
        depth = 1;
    }

    std::uint64_t nodes = 0ULL;
    const auto start = std::chrono::steady_clock::now();
    for (int i = 1; i <= depth; ++i) {
        nodes = pos.perft(i);
        const auto finish = std::chrono::steady_clock::now();
        const std::chrono::duration<double> elapsed = finish - start;

        // clang-format off
        std::cout << "info"
                  << " depth " << i
                  << " nodes " << nodes
                  << " time " << static_cast<int>(elapsed.count() * 1000)
                  << " nps " << static_cast<std::uint64_t>(nodes / elapsed.count())
                  << std::endl;
        // clang-format on
    }

    std::cout << "nodes " << nodes << std::endl;
}

}  // namespace common
