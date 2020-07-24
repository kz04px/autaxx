#ifndef TRYHARD_TTENTRY_HPP
#define TRYHARD_TTENTRY_HPP

#include <cassert>
#include <cstdint>
#include <libataxx/move.hpp>

struct TTEntry {
    enum class Flag : std::uint8_t
    {
        Exact = 0,
        Lower,
        Upper
    };

    [[nodiscard]] constexpr bool operator==(const TTEntry &rhs) const noexcept {
        return hash == rhs.hash && move == rhs.move && score == rhs.score && depth == rhs.depth && flag == rhs.flag;
    }

    std::uint64_t hash;
    libataxx::Move move;
    std::int16_t score;
    std::uint8_t depth;
    Flag flag;
};

static_assert(sizeof(TTEntry) == 16);

#endif
