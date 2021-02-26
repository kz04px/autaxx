#include <libataxx/position.hpp>
#include "../score.hpp"
#include "nnue.hpp"
#include "phase.hpp"

namespace search::nnue {

// Return the evaluation of the position from the side to move's point of view
int NNUE::eval() const noexcept {
    return static_cast<int>(600.0f * m_network.run(m_accumulator));
}

// Return the evaluation of the position from the side to move's point of view
int NNUE::eval(const libataxx::Position &pos) const noexcept {
    return static_cast<int>(600.0f * m_network.run(pos));
}

}  // namespace search::nnue
