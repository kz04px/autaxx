#include <libataxx/position.hpp>
#include "alphabeta.hpp"

namespace search {

namespace alphabeta {

// clang-format off
constexpr int pst[49] = {
    60, 40, 20, 20, 20, 40, 60,
    40, 20, 10, 10, 10, 20, 40,
    20, 10,  0,  0,  0, 10, 20,
    20, 10,  0,  0,  0, 10, 20,
    20, 10,  0,  0,  0, 10, 20,
    40, 20, 10, 10, 10, 20, 40,
    60, 40, 20, 20, 20, 40, 60,
};
// clang-format on

constexpr int hole_penalties[] = {0, 0, 25, 50, 100, 200, 300, 400, 500};

[[nodiscard]] constexpr int eval_us(const libataxx::Bitboard &us,
                                    const libataxx::Bitboard &them,
                                    const libataxx::Bitboard &empty) {
    int score = 0;

    // Material
    score += 100 * us.count();

    // PST
    for (const auto &sq : us) {
        score += pst[static_cast<int>(sq)];
    }

    // Holes
    for (const auto &sq : us.singles() & empty) {
        const auto bb = libataxx::Bitboard{sq};
        const auto neighbours = bb.singles() & us;
        score -= hole_penalties[neighbours.count()];
    }

    // Surrounded
    score += 15 * (us & (~(empty.singles()))).count();

    return score;
}

// Return the evaluation of the position from the side to move's point of view
int Alphabeta::eval(const libataxx::Position &pos) noexcept {
    int score = 0;

    score += eval_us(pos.us(), pos.them(), pos.empty());
    score -= eval_us(pos.them(), pos.us(), pos.empty());

    return score;
}

}  // namespace alphabeta

}  // namespace search
