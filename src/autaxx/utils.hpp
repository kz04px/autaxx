#ifndef UTILS_HPP
#define UTILS_HPP

#include <cstdint>
#include <random>

namespace {

static std::random_device rd;
static std::mt19937 eng(rd());

}  // namespace

namespace utils {

inline std::uint32_t rand_u32(const std::uint32_t lower,
                              const std::uint32_t upper) {
    std::uniform_int_distribution<std::uint32_t> uni(lower, upper);
    return uni(eng);
}

inline std::uint64_t rand_u64(const std::uint64_t lower,
                              const std::uint64_t upper) {
    std::uniform_int_distribution<std::uint64_t> uni(lower, upper);
    return uni(eng);
}

}  // namespace utils

#endif
