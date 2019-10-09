#include "position.hpp"
#include <iostream>
#include "../../options.hpp"
#include "moves.hpp"

namespace UAI {

// Set the current position
// -- position startpos
// -- position startpos moves a3 d2d4
// -- position fen x5o/7/2-1-2/7/2-1-2/7/o5x x 0 1
// -- position fen x5o/7/2-1-2/7/2-1-2/7/o5x x 0 1 moves a3 d2d4
void position(libataxx::Position &pos, std::stringstream &stream) {
    std::string word = "";
    std::string fen = "";
    stream >> word;

    // Position
    if (word == "startpos") {
        fen = "startpos";
        stream >> word;
    } else if (word == "fen") {
        // Collect fen string
        while (stream >> word && word != "moves") {
            if (fen != "") {
                fen += " ";
            }
            fen += word;
        }
    } else {
        if (Options::checks["debug"].get()) {
            std::cout << "info unknown UAI::position term \"" << word << "\""
                      << std::endl;
        }
        return;
    }

    pos.set_fen(fen);

    // Return if we didn't run into a move string
    if (word != "moves") {
        return;
    }

    moves(pos, stream);
}

}  // namespace UAI
