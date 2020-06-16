#include "split.hpp"
#include <cstdint>
#include <iostream>
#include <libataxx/move.hpp>
#include <sstream>

namespace UAI {

namespace Extension {

// Perform a split perft
void split(const libataxx::Position &pos, std::stringstream &stream) {
    int depth = 0;
    stream >> depth;
    if (depth < 1) {
        depth = 1;
    }

    std::uint64_t total_nodes = 0ULL;
    libataxx::Move moves[libataxx::max_moves];
    const int num_moves = pos.legal_moves(moves);

    for (int i = 0; i < num_moves; ++i) {
        auto npos = pos;
        npos.makemove(moves[i]);

        const std::uint64_t nodes = npos.perft(depth - 1);
        total_nodes += nodes;

        std::cout << moves[i] << " " << nodes << std::endl;
    }
    std::cout << "nodes " << total_nodes << std::endl;
}

}  // namespace Extension

}  // namespace UAI
