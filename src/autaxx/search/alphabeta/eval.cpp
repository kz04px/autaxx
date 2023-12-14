#include <libataxx/position.hpp>
#include "alphabeta.hpp"

namespace search::alphabeta {

// Return the evaluation of the position from the side to move's point of view
int Alphabeta::eval(const libataxx::Position &pos) noexcept {
    const int num_us = pos.get_us().count();
    const int num_them = pos.get_them().count();
    return 100 * (num_us - num_them);
}

}  // namespace search::alphabeta
