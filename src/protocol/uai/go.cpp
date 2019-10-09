#include "go.hpp"
#include <iostream>
#include <thread>
#include "../../options.hpp"
#include "../../search/search.hpp"
#include "../../search/settings.hpp"

std::thread search_thread;
volatile bool search_stop = false;

namespace UAI {

// Stop searching
void stop() {
    search_stop = true;
    if (search_thread.joinable()) {
        search_thread.join();
    }
    search_stop = false;
}

// Start searching for a best move (threaded)
void go(const libataxx::Position &pos, std::stringstream &stream) {
    stop();

    search::Settings options;
    std::string word;

    while (stream >> word) {
        // Node search
        if (word == "nodes") {
            options.type = search::Type::Nodes;
            stream >> options.nodes;
        }
        // Depth search
        else if (word == "depth") {
            options.type = search::Type::Depth;
            stream >> options.depth;
        }
        // Infinite search
        else if (word == "infinite") {
            options.type = search::Type::Infinite;
        }
        // Movetime
        else if (word == "movetime") {
            options.type = search::Type::Movetime;
            stream >> options.movetime;
        }
        // Time search
        else if (word == "btime") {
            options.type = search::Type::Time;
            stream >> options.btime;
        } else if (word == "wtime") {
            options.type = search::Type::Time;
            stream >> options.wtime;
        } else if (word == "binc") {
            options.type = search::Type::Time;
            stream >> options.binc;
        } else if (word == "winc") {
            options.type = search::Type::Time;
            stream >> options.winc;
        } else if (word == "movestogo") {
            options.type = search::Type::Time;
            stream >> options.movestogo;
        } else {
            if (Options::checks["debug"].get()) {
                std::cout << "info unknown UAI::go term \"" << word << "\""
                          << std::endl;
            }
        }
    }

    if (Options::combos["search"].get() == "minimax") {
        search_thread =
            std::thread(search::minimax::root, pos, options, &search_stop);
    } else if (Options::combos["search"].get() == "random") {
        search::random::root(pos);
    } else if (Options::combos["search"].get() == "alphabeta") {
        search_thread =
            std::thread(search::alphabeta::root, pos, options, &search_stop);
    }
}

}  // namespace UAI
