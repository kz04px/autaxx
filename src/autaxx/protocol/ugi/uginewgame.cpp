#include "uginewgame.hpp"
#include "../../search/search.hpp"

namespace UGI {

// New game started
void uginewgame(libataxx::Position &pos) {
    search::search_main->clear();
    pos.set_fen("startpos");
}

}  // namespace UGI
