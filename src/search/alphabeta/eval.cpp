#include <libataxx/position.hpp>
#include "alphabeta.hpp"

namespace search {

namespace alphabeta {

// Return the evaluation of the position from the side to move's point of view
int Alphabeta::eval(const libataxx::Position &pos) noexcept {
    const int num_us = pos.us().count();
    const int num_them = pos.them().count();
    return 100 * (num_us - num_them);
}

}  // namespace alphabeta

}  // namespace search
