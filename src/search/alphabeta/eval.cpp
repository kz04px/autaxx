#include <libataxx/position.hpp>
#include "alphabeta.hpp"

namespace search {

namespace alphabeta {

// clang-format off
constexpr int pst[49] = {
    30, 20, 10, 10, 10, 20, 30,
    20, 10, 10,  5, 10, 10, 20,
    10, 10,  5,  0,  5, 10, 10,
    10,  5,  0,  0,  0,  5, 10,
    10, 10,  5,  0,  5, 10, 10,
    20, 10, 10,  5, 10, 10, 20,
    30, 20, 10, 10, 10, 20, 30,
};
// clang-format on

constexpr int hole_penalties[] = {0, 0, 0, 0, 100, 200, 300, 400, 500};

// Return the evaluation of the position from the side to move's point of view
int Alphabeta::eval(const libataxx::Position &pos) noexcept {
    int score = 0;

    // Material
    score += 100 * pos.us().count();
    score -= 100 * pos.them().count();

    // PST
    for (const auto &sq : pos.us()) {
        score += pst[static_cast<int>(sq)];
    }
    for (const auto &sq : pos.them()) {
        score -= pst[static_cast<int>(sq)];
    }

    const auto reachable_them = pos.them().singles() | pos.them().doubles();
    for (const auto &sq : pos.us().singles() & pos.empty() & reachable_them) {
        const auto bb = libataxx::Bitboard{sq};
        const auto neighbours = bb.singles() & pos.us();
        score -= hole_penalties[neighbours.count()];
    }

    const auto reachable_us = pos.us().singles() | pos.us().doubles();
    for (const auto &sq : pos.them().singles() & pos.empty() & reachable_us) {
        const auto bb = libataxx::Bitboard{sq};
        const auto neighbours = bb.singles() & pos.them();
        score += hole_penalties[neighbours.count()];
    }

    return score;
}

}  // namespace alphabeta

}  // namespace search
