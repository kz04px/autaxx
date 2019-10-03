#ifndef SEARCH_HPP
#define SEARCH_HPP

#include <chrono>
#include <cstdint>
#include <libataxx/move.hpp>
#include <libataxx/position.hpp>
#include <vector>

#define MAX_DEPTH 128
#define MATE_SCORE 100000

enum SearchType
{
    Time = 0,
    Depth,
    Nodes,
    Movetime,
    Infinite
};

struct SearchOptions {
    int type = SearchType::Time;
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

struct SearchController {
    volatile bool *stop;
    std::uint64_t max_nodes;
    std::chrono::high_resolution_clock::time_point end_time;
};

struct SearchStats {
    std::uint64_t nodes = 0ULL;
    std::uint64_t tt_hits = 0ULL;
    std::uint64_t tb_hits = 0ULL;
    int depth = 0;
    int seldepth = 0;
};

typedef std::vector<libataxx::Move> PV;

struct SearchStack {
    int ply = 0;
    PV pv;
};

int minimax(SearchController &controller,
            SearchStats &stats,
            SearchStack *stack,
            const libataxx::Position &pos,
            const int depth);
void search(const libataxx::Position &pos,
            const SearchOptions &options,
            volatile bool *stop);
bool legal_pv(const libataxx::Position &pos, const PV &pv);

#endif
