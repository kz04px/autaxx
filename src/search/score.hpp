#ifndef SCORE_HPP
#define SCORE_HPP

#include <array>
#include <libataxx/position.hpp>

class Score {
   public:
    using value_type = int;

    constexpr Score() : m_data{} {
    }

    constexpr Score(const value_type mg, const value_type eg) : m_data{mg, eg} {
    }

    [[nodiscard]] constexpr Score operator+(const Score &rhs) const noexcept {
        return {m_data[0] + rhs.m_data[0], m_data[1] + rhs.m_data[1]};
    }

    [[nodiscard]] constexpr Score operator-(const Score &rhs) const noexcept {
        return {m_data[0] - rhs.m_data[0], m_data[1] - rhs.m_data[1]};
    }

    [[nodiscard]] constexpr Score operator-() const noexcept {
        return {-m_data[0], -m_data[1]};
    }

    [[nodiscard]] constexpr Score operator*(const value_type n) const noexcept {
        return {m_data[0] * n, m_data[1] * n};
    }

    constexpr Score operator+=(const Score &rhs) noexcept {
        m_data[0] += rhs.m_data[0];
        m_data[1] += rhs.m_data[1];
        return *this;
    }

    constexpr Score operator-=(const Score &rhs) noexcept {
        m_data[0] -= rhs.m_data[0];
        m_data[1] -= rhs.m_data[1];
        return *this;
    }

    [[nodiscard]] constexpr bool operator==(const Score &rhs) const noexcept {
        return m_data[0] == rhs.m_data[0] && m_data[1] == rhs.m_data[1];
    }

    [[nodiscard]] constexpr bool operator!=(const Score &rhs) const noexcept {
        return m_data[0] != rhs.m_data[0] || m_data[1] != rhs.m_data[1];
    }

    [[nodiscard]] constexpr value_type average() const noexcept {
        return (mg() + eg()) / 2;
    }

    [[nodiscard]] constexpr value_type mg() const noexcept {
        return m_data[0];
    }

    [[nodiscard]] constexpr value_type eg() const noexcept {
        return m_data[1];
    }

   private:
    std::array<value_type, 2> m_data;
};

static_assert(sizeof(Score) == 2 * sizeof(int));
static_assert(Score{} == Score{0, 0});
static_assert(Score{} != Score{1, 0});
static_assert(Score{} != Score{0, 1});
static_assert(Score{1, 2}.mg() == 1);
static_assert(Score{1, 2}.eg() == 2);
static_assert(Score{1, 2} + Score{3, 4} == Score{4, 6});
static_assert(Score{1, 2} * 2 == Score{2, 4});
static_assert(-Score{1, 2} == Score{-1, -2});

#endif
