#ifndef SEARCH_TRYHARD_PHASE_HPP
#define SEARCH_TRYHARD_PHASE_HPP

#include <libataxx/position.hpp>

namespace search::tryhard {

[[nodiscard]] constexpr float phase(const libataxx::Position &pos) {
    const int both = (pos.get_us() | pos.get_them()).count();
    const int all = (~pos.get_gaps()).count();
    return static_cast<float>(both) / all;
}

}  // namespace search::tryhard

#endif
