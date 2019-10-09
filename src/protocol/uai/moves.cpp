#include "moves.hpp"
#include <iostream>
#include <libataxx/move.hpp>
#include "../../options.hpp"

namespace UAI {

// Apply a series of moves to the position
// -- moves a3 d2d4
void moves(libataxx::Position &pos, std::stringstream &stream) {
    std::string word = "";
    while (stream >> word) {
        libataxx::Move move;
        try {
            move = libataxx::Move::from_uai(word);
        } catch (...) {
            if (Options::checks["debug"].get()) {
                std::cout << "info string failed to parse move \"" << word
                          << "\"" << std::endl;
            }
            continue;
        }

        if (pos.legal_move(move)) {
            pos.makemove(move);
        } else {
            if (Options::checks["debug"].get()) {
                std::cout << "info string illegal move \"" << move << "\""
                          << std::endl;
            }
        }
    }
}

}  // namespace UAI
