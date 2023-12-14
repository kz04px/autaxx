#include <libataxx/position.hpp>
#include "../score.hpp"
#include "phase.hpp"
#include "tryhard.hpp"

namespace search::tryhard {

// clang-format off
constexpr Score<int> pst[49] = {
    {100, 95},  {73, 65},  {15, 87},  {12, 37},  {33, 84},  {57, 74},  {56, 96},
     {86, 57},  {19, 37},   {2, 18},    {1, 0},   {2, 13},  {18, 27},  {41, 74},
     {18, 74},   {3, 15},   {0, 21},    {4, 2},  {15, 29},   {1, 23},  {15, 85},
     {15, 44},    {5, 2},    {2, 0},    {1, 0},    {4, 5},    {3, 2},  {28, 26},
     {18, 81},   {1, 19},   {14, 9},    {5, 4},   {2, 28},   {5, 19},  {16, 83},
     {65, 67},  {25, 33},   {9, 33},    {2, 1},   {3, 19},  {25, 34},  {56, 69},
    {78, 100},  {37, 78},  {27, 72},  {20, 45},  {26, 80},  {73, 65},  {83, 85},
};
// clang-format on

constexpr Score<int> piece_value = {100, 79};
constexpr Score<int> surrounded = {149, 27};
constexpr Score<int> turn_bonus = {243, 394};
constexpr Score<int> hole_penalties[] = {
    {-17, 23},
    {4, 28},
    {-28, 23},
    {-88, 0},
    {-125, -64},
    {-200, -129},
    {-322, -192},
    {-446, -351},
    {-534, -440},
};

[[nodiscard]] constexpr Score<int> eval_us(const libataxx::Bitboard &us,
                                           const libataxx::Bitboard &them,
                                           const libataxx::Bitboard &empty) {
    Score<int> score;
    const auto reachable = them.singles() | them.doubles();

    // Material
    score += piece_value * us.count();

    // PST
    for (const auto &sq : us) {
        score += pst[sq.index()];
    }

    // Holes
    for (const auto &sq : us.singles() & empty & reachable) {
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

    score += eval_us(pos.get_us(), pos.get_them(), pos.get_empty());
    score -= eval_us(pos.get_them(), pos.get_us(), pos.get_empty());

    // Turn bonus
    score += turn_bonus;

    return score.mg() * (1.0f - p) + score.eg() * p;
}

}  // namespace search::tryhard
