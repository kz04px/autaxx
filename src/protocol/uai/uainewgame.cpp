#include "uainewgame.hpp"
#include "../../search/search.hpp"

namespace UAI {

// New game started
void uainewgame(libataxx::Position &pos) {
    search::search_main->clear();
    pos.set_fen("startpos");
}

}  // namespace UAI
