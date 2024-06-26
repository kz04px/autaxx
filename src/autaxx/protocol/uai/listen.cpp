#include <iostream>
#include <memory>
#include "../../options.hpp"
#include "../../search/alphabeta/alphabeta.hpp"
#include "../../search/leastcaptures/leastcaptures.hpp"
#include "../../search/mcts/mcts.hpp"
#include "../../search/minimax/minimax.hpp"
#include "../../search/mostcaptures/mostcaptures.hpp"
#include "../../search/nnue/nnue.hpp"
#include "../../search/random/random.hpp"
#include "../../search/tryhard/tryhard.hpp"
#include "../../version.hpp"
#include "../common/display.hpp"
#include "../common/moves.hpp"
#include "../common/perft.hpp"
#include "../common/position.hpp"
#include "../common/split.hpp"
#include "../protocol.hpp"
#include "go.hpp"
#include "isready.hpp"
#include "setoption.hpp"
#include "uainewgame.hpp"

using namespace search;

namespace UAI {

// Communicate with the UAI protocol (Universal Ataxx Interface)
// Based on the UCI protocol (Universal Chess Interface)
void listen() {
    std::cout << "id name Autaxx " << version_major << "." << version_minor << std::endl;
    std::cout << "id author kz04px" << std::endl;

    // Create options
    Options::checks["debug"] = Options::Check(false);
    Options::spins["hash"] = Options::Spin(1, 65536, 128);
    Options::strings["nnue-path"] = Options::String("./weights-8b5d1db5176c.nnue");
    Options::combos["search"] = Options::Combo("tryhard",
                                               {
                                                   "tryhard",
                                                   "mcts",
                                                   "minimax",
                                                   "mostcaptures",
                                                   "random",
                                                   "leastcaptures",
                                                   "nnue",
                                                   "alphabeta",
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
    } else if (Options::combos["search"].get() == "tryhard") {
        search_main = std::unique_ptr<Search>(new tryhard::Tryhard(Options::spins["hash"].get()));
    } else if (Options::combos["search"].get() == "mcts") {
        search_main = std::unique_ptr<Search>(new mcts::MCTS());
    } else if (Options::combos["search"].get() == "minimax") {
        search_main = std::unique_ptr<Search>(new minimax::Minimax());
    } else if (Options::combos["search"].get() == "alphabeta") {
        search_main = std::unique_ptr<Search>(new alphabeta::Alphabeta());
    } else if (Options::combos["search"].get() == "leastcaptures") {
        search_main = std::unique_ptr<Search>(new leastcaptures::LeastCaptures());
    } else if (Options::combos["search"].get() == "nnue") {
        // Check weight file exists
        std::ifstream f(Options::strings["nnue-path"].get().c_str());
        if (!f.good()) {
            std::cerr << "NNUE weight file could not be accessed\n";
            return;
        }

        search_main =
            std::unique_ptr<Search>(new nnue::NNUE(Options::strings["nnue-path"].get(), Options::spins["hash"].get()));
    }

    if (!search_main) {
        std::cerr << "Failed to allocate search" << std::endl;
        return;
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
            common::perft(pos, stream);
        } else if (word == "split") {
            common::split(pos, stream);
        } else if (word == "position") {
            common::position(pos, stream);
        } else if (word == "moves") {
            common::moves(pos, stream);
        } else if (word == "go") {
            go(pos, stream);
        } else if (word == "stop") {
            stop();
        } else if (word == "print" || word == "display") {
            common::display(pos);
        } else if (word == "quit") {
            break;
        } else {
            if (Options::checks["debug"].get()) {
                std::cout << "info unknown UAI command \"" << word << "\"" << std::endl;
            }
        }
    }

    stop();
}

}  // namespace UAI
