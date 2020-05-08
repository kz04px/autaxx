#include "eval.hpp"
#include "phase.hpp"

namespace search::mcts {

constexpr std::pair<int, int> operator+(const std::pair<int, int> &x,
                                        const std::pair<int, int> &y) {
    return std::make_pair(x.first + y.first, x.second + y.second);
}

constexpr std::pair<int, int> operator*(const std::pair<int, int> &x,
                                        const int n) {
    return std::make_pair(x.first * n, x.second * n);
}

constexpr std::pair<int, int> operator+=(std::pair<int, int> &x,
                                         const std::pair<int, int> &y) {
    x.first += y.first;
    x.second += y.second;
    return x;
}

constexpr std::pair<int, int> operator-=(std::pair<int, int> &x,
                                         const std::pair<int, int> &y) {
    x.first -= y.first;
    x.second -= y.second;
    return x;
}

// clang-format off
constexpr std::pair<int, int> pst[49] = {
    {60,90}, {40,60}, {20,40}, {20,40}, {20,40}, {40,60}, {60,90},
    {40,60}, {20,30}, {10,15}, {10,15}, {10,15}, {20,30}, {40,60},
    {20,40}, {10,15},   {0,0},   {0,0},   {0,0}, {10,15}, {20,40},
    {20,40}, {10,15},   {0,0},   {0,0},   {0,0}, {10,15}, {20,40},
    {20,40}, {10,15},   {0,0},   {0,0},   {0,0}, {10,15}, {20,40},
    {40,60}, {20,30}, {10,15}, {10,15}, {10,15}, {20,30}, {40,60},
    {60,90}, {40,60}, {20,40}, {20,40}, {20,40}, {40,60}, {60,90},
};
// clang-format on

constexpr std::pair<int, int> piece_value = {100, 100};
constexpr std::pair<int, int> surrounded = {15, 15};
constexpr std::pair<int, int> turn_bonus = {200, 200};
constexpr std::pair<int, int> hole_penalties[] = {
    {0, 0},
    {0, 0},
    {25, 25},
    {50, 50},
    {100, 100},
    {200, 200},
    {300, 300},
    {400, 400},
    {500, 500},
};

[[nodiscard]] constexpr std::pair<int, int> eval_us(
    const libataxx::Bitboard &us,
    const libataxx::Bitboard &them,
    const libataxx::Bitboard &empty) {
    std::pair<int, int> score;

    // Material
    score += piece_value * us.count();

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
    score += surrounded * (us & (~(empty.singles()))).count();

    return score;
}

// Return the evaluation of the position from the side to move's point of view
int eval(const libataxx::Position &pos) {
    const auto p = phase(pos);
    std::pair<int, int> score;

    score += eval_us(pos.us(), pos.them(), pos.empty());
    score -= eval_us(pos.them(), pos.us(), pos.empty());

    // Turn bonus
    score += turn_bonus;

    return score.first * (1.0 - p) + score.second * p;
}

}  // namespace search::mcts
