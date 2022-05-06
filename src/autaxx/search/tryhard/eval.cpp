#include <libataxx/position.hpp>
#include "../score.hpp"
#include "phase.hpp"
#include "tryhard.hpp"

namespace search::tryhard {

// clang-format off
constexpr Score<int> pst[49] = {
    {60,90}, {40,60}, {20,40}, {20,40}, {20,40}, {40,60}, {60,90},
    {40,60}, {20,30}, {10,15}, {10,15}, {10,15}, {20,30}, {40,60},
    {20,40}, {10,15},   {0,0},   {0,0},   {0,0}, {10,15}, {20,40},
    {20,40}, {10,15},   {0,0},   {0,0},   {0,0}, {10,15}, {20,40},
    {20,40}, {10,15},   {0,0},   {0,0},   {0,0}, {10,15}, {20,40},
    {40,60}, {20,30}, {10,15}, {10,15}, {10,15}, {20,30}, {40,60},
    {60,90}, {40,60}, {20,40}, {20,40}, {20,40}, {40,60}, {60,90},
};
// clang-format on

constexpr Score<int> piece_value = {100, 100};
constexpr Score<int> surrounded = {50, 50};
constexpr Score<int> turn_bonus = {200, 200};
constexpr Score<int> hole_penalties[] = {
    {0, 0},
    {0, 0},
    {-25, -25},
    {-50, -50},
    {-100, -100},
    {-200, -200},
    {-300, -300},
    {-400, -400},
    {-500, -500},
};

[[nodiscard]] constexpr Score<int> eval_us(const libataxx::Bitboard &us,
                                           const libataxx::Bitboard &,
                                           const libataxx::Bitboard &empty) {
    Score<int> score;

    // Material
    score += piece_value * us.count();

    // PST
    for (const auto &sq : us) {
        score += pst[sq.index()];
    }

    // Holes
    for (const auto &sq : us.singles() & empty) {
        const auto bb = libataxx::Bitboard{sq};
        const auto neighbours = bb.singles() & us;
        score += hole_penalties[neighbours.count()];
    }

    // Surrounded
    score += surrounded * (us & (~(empty.singles()))).count();

    return score;
}

// Return the evaluation of the position from the side to move's point of view
[[nodiscard]] int Tryhard::eval(const libataxx::Position &pos) noexcept {
    const auto p = phase(pos);
    Score<int> score;

    score += eval_us(pos.us(), pos.them(), pos.empty());
    score -= eval_us(pos.them(), pos.us(), pos.empty());

    // Turn bonus
    score += turn_bonus;

    return score.mg() * (1.0f - p) + score.eg() * p;
}

}  // namespace search::tryhard
