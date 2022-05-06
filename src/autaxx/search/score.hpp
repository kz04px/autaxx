#ifndef SCORE_HPP
#define SCORE_HPP

#include <array>

template <typename T>
class Score {
   public:
    [[nodiscard]] constexpr Score() : m_data{} {
    }

    [[nodiscard]] constexpr Score(const T mg, const T eg) : m_data{mg, eg} {
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

    template <typename B>
    [[nodiscard]] constexpr Score operator*(const B n) const noexcept {
        return {static_cast<T>(m_data[0] * n), static_cast<T>(m_data[1] * n)};
    }

    template <typename B>
    [[nodiscard]] constexpr Score operator/(const B n) const noexcept {
        return {static_cast<T>(m_data[0] / n), static_cast<T>(m_data[1] / n)};
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

    [[nodiscard]] constexpr T average() const noexcept {
        return (mg() + eg()) / 2;
    }

    [[nodiscard]] constexpr T mg() const noexcept {
        return m_data[0];
    }

    [[nodiscard]] constexpr T eg() const noexcept {
        return m_data[1];
    }

    [[nodiscard]] auto begin() noexcept {
        return m_data.begin();
    }

    [[nodiscard]] auto end() noexcept {
        return m_data.end();
    }

    [[nodiscard]] auto begin() const noexcept {
        return m_data.begin();
    }

    [[nodiscard]] auto end() const noexcept {
        return m_data.end();
    }

   private:
    std::array<T, 2> m_data;
};

template <typename T>
inline std::ostream &operator<<(std::ostream &os, const Score<T> &s) noexcept {
    os << s.mg() << "," << s.eg();
    return os;
}

static_assert(sizeof(Score<int>) == 2 * sizeof(int));
static_assert(Score<int>{} == Score<int>{0, 0});
static_assert(Score<int>{} != Score<int>{1, 0});
static_assert(Score<int>{} != Score<int>{0, 1});
static_assert(Score<int>{1, 2}.mg() == 1);
static_assert(Score<int>{1, 2}.eg() == 2);
static_assert(Score<int>{1, 2} + Score<int>{3, 4} == Score<int>{4, 6});
static_assert(Score<int>{1, 2} - Score<int>{3, 4} == Score<int>{-2, -2});
static_assert(-Score<int>{1, 2} == Score<int>{-1, -2});
static_assert(Score<int>{2, 4} * 2 == Score<int>{4, 8});
static_assert(Score<int>{2, 4} / 2 == Score<int>{1, 2});

static_assert(sizeof(Score<float>) == 2 * sizeof(float));
static_assert(Score<float>{} == Score<float>{0.0f, 0.0f});
static_assert(Score<float>{} != Score<float>{1.0f, 0.0f});
static_assert(Score<float>{} != Score<float>{0.0f, 1.0f});
static_assert(Score<float>{1.0f, 2.0f}.mg() == 1.0f);
static_assert(Score<float>{1.0f, 2.0f}.eg() == 2.0f);

#endif
