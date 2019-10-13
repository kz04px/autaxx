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

    return score;
}

}  // namespace alphabeta

}  // namespace search
