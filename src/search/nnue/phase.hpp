#ifndef SEARCH_NNUE_PHASE_HPP
#define SEARCH_NNUE_PHASE_HPP

#include <libataxx/position.hpp>

namespace search::nnue {

constexpr float phase(const libataxx::Position &pos) {
    const int both = (pos.us() | pos.them()).count();
    const int all = (~pos.gaps()).count();
    return static_cast<float>(both) / all;
}

}  // namespace search::nnue

#endif
