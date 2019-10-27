#include "go.hpp"
#include <iostream>
#include <thread>
#include "../../options.hpp"
#include "../../search/search.hpp"

using namespace search;

namespace UAI {

// Stop searching
void stop() {
    search_main->stop();
}

// Start searching for a best move (threaded)
void go(const libataxx::Position &pos, std::stringstream &stream) {
    stop();

    Settings options;
    std::string word;

    while (stream >> word) {
        // Node search
        if (word == "nodes") {
            options.type = Type::Nodes;
            stream >> options.nodes;
        }
        // Depth search
        else if (word == "depth") {
            options.type = Type::Depth;
            stream >> options.depth;
        }
        // Infinite search
        else if (word == "infinite") {
            options.type = Type::Infinite;
        }
        // Movetime
        else if (word == "movetime") {
            options.type = Type::Movetime;
            stream >> options.movetime;
        }
        // Time search
        else if (word == "btime") {
            options.type = Type::Time;
            stream >> options.btime;
        } else if (word == "wtime") {
            options.type = Type::Time;
            stream >> options.wtime;
        } else if (word == "binc") {
            options.type = Type::Time;
            stream >> options.binc;
        } else if (word == "winc") {
            options.type = Type::Time;
            stream >> options.winc;
        } else if (word == "movestogo") {
            options.type = Type::Time;
            stream >> options.movestogo;
        } else {
            if (Options::checks["debug"].get()) {
                std::cout << "info unknown UAI::go term \"" << word << "\""
                          << std::endl;
            }
        }
    }

    search_main->go(pos, options);
}

}  // namespace UAI
