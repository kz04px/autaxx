#include <iostream>
#include <memory>
#include "../../options.hpp"
#include "../../search/alphabeta/alphabeta.hpp"
#include "../../search/leastcaptures/leastcaptures.hpp"
#include "../../search/mcts/mcts.hpp"
#include "../../search/minimax/minimax.hpp"
#include "../../search/mostcaptures/mostcaptures.hpp"
#include "../../search/random/random.hpp"
#include "../protocol.hpp"
#include "extension/display.hpp"
#include "extension/perft.hpp"
#include "extension/split.hpp"
#include "go.hpp"
#include "isready.hpp"
#include "moves.hpp"
#include "position.hpp"
#include "setoption.hpp"
#include "uainewgame.hpp"

using namespace search;

namespace UAI {

// Communicate with the UAI protocol (Universal Ataxx Interface)
// Based on the UCI protocol (Universal Chess Interface)
void listen() {
    std::cout << "id name Autaxx" << std::endl;
    std::cout << "id author kz04px" << std::endl;

    // Create options
    Options::checks["debug"] = Options::Check(false);
    Options::spins["hash"] = Options::Spin(1, 2048, 128);
    Options::combos["search"] = Options::Combo("alphabeta",
                                               {
                                                   "alphabeta",
                                                   "mcts",
                                                   "minimax",
                                                   "mostcaptures",
                                                   "random",
                                                   "leastcaptures",
                                               });

    Options::print();

    std::cout << "uaiok" << std::endl;

    std::string word;
    std::string line;

    // Wait for isready before we do anything else
    // The engine might be opened just to check if it works
    while (true) {
        std::getline(std::cin, line);
        std::stringstream stream{line};
        stream >> word;

        if (word == "isready") {
            break;
        } else if (word == "setoption") {
            setoption(stream);
        } else if (word == "quit") {
            return;
        }
    }

    // Set search type
    if (Options::combos["search"].get() == "random") {
        search_main = std::unique_ptr<Search>(new random::Random());
    } else if (Options::combos["search"].get() == "mostcaptures") {
        search_main = std::unique_ptr<Search>(new mostcaptures::MostCaptures());
    } else if (Options::combos["search"].get() == "alphabeta") {
        search_main = std::unique_ptr<Search>(
            new alphabeta::Alphabeta(Options::spins["hash"].get()));
    } else if (Options::combos["search"].get() == "mcts") {
        search_main = std::unique_ptr<Search>(new mcts::MCTS());
    } else if (Options::combos["search"].get() == "minimax") {
        search_main = std::unique_ptr<Search>(new minimax::Minimax());
    } else if (Options::combos["search"].get() == "leastcaptures") {
        search_main =
            std::unique_ptr<Search>(new leastcaptures::LeastCaptures());
    }

    libataxx::Position pos;
    uainewgame(pos);

    isready();

    // isready received, now we're ready to do something
    bool quit = false;
    while (!quit) {
        std::getline(std::cin, line);
        std::stringstream stream{line};
        stream >> word;

        if (word == "uainewgame") {
            uainewgame(pos);
        } else if (word == "isready") {
            isready();
        } else if (word == "perft") {
            Extension::perft(pos, stream);
        } else if (word == "split") {
            Extension::split(pos, stream);
        } else if (word == "position") {
            position(pos, stream);
        } else if (word == "moves") {
            moves(pos, stream);
        } else if (word == "go") {
            go(pos, stream);
        } else if (word == "stop") {
            stop();
        } else if (word == "print") {
            Extension::display(pos);
        } else if (word == "display") {
            Extension::display(pos);
        } else if (word == "quit") {
            break;
        } else {
            if (Options::checks["debug"].get()) {
                std::cout << "info unknown UAI command \"" << word << "\""
                          << std::endl;
            }
        }
    }

    stop();
}

}  // namespace UAI
