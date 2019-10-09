#include "uainewgame.hpp"

namespace UAI {

// New game started
void uainewgame(libataxx::Position &pos) {
    // TODO:
    // -- Clear TT
    pos.set_fen("startpos");
}

}  // namespace UAI
