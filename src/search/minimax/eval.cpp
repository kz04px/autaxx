#include <libataxx/position.hpp>
#include "minimax.hpp"

namespace search {

namespace minimax {

// Return the evaluation of the position from the side to move's point of view
int Minimax::eval(const libataxx::Position &pos) noexcept {
    const int num_us = pos.us().count();
    const int num_them = pos.them().count();
    return 100 * (num_us - num_them);
}

}  // namespace minimax

}  // namespace search
