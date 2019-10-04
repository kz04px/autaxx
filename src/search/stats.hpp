#ifndef SEARCH_STATS_HPP
#define SEARCH_STATS_HPP

#include <cstdint>

namespace search {

struct Stats {
    std::uint64_t nodes = 0ULL;
    std::uint64_t tt_hits = 0ULL;
    std::uint64_t tb_hits = 0ULL;
    int depth = 0;
    int seldepth = 0;
};

}  // namespace search

#endif
