#ifndef SEARCH_SETTINGS_HPP
#define SEARCH_SETTINGS_HPP

#include <cstdint>

namespace search {

enum Type
{
    Time = 0,
    Depth,
    Nodes,
    Movetime,
    Infinite
};

struct Settings {
    int type = Type::Time;
    // Time search
    int btime = -1;
    int wtime = -1;
    int binc = -1;
    int winc = -1;
    int movestogo = -1;
    // Movetime search
    int movetime = -1;
    // Nodes search
    std::uint64_t nodes = -1;
    // Depth search
    int depth = -1;
};

}  // namespace search

#endif
