#ifndef SEARCH_TRYHARD_PHASE_HPP
#define SEARCH_TRYHARD_PHASE_HPP

#include <libataxx/position.hpp>

namespace search {

namespace tryhard {

constexpr float phase(const libataxx::Position &pos) {
    const int both = (pos.us() | pos.them()).count();
    const int all = (~pos.gaps()).count();
    return static_cast<float>(both) / all;
}

}  // namespace tryhard

}  // namespace search

#endif
