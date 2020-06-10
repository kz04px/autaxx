#include <catch2/catch.hpp>
#include <chrono>
#include <libataxx/move.hpp>
#include <libataxx/position.hpp>
#include "../src/search/minimax/minimax.hpp"

#define MAX_DEPTH 128
#define MATE_SCORE 100000

using namespace std::chrono;

TEST_CASE("search::minimax() -- Mate in 1") {
    const std::pair<std::string, std::string> tests[] = {
        {"7/7/7/3o3/7/7/x6 o 0", "d4b2"},
        {"7/7/7/3x3/7/7/o6 x 0", "d4b2"},
        {"7/7/7/3o3/7/7/x6 x 0", "a1c3"},
        {"7/7/7/3x3/7/7/o6 o 0", "a1c3"},
    };

    volatile bool stop = false;
    const auto start_time = high_resolution_clock::now();
    search::Stats stats;
    search::Stack stack[MAX_DEPTH + 1];
    for (int i = 0; i < MAX_DEPTH + 1; ++i) {
        stack[i].ply = i;
    }
    search::Controller controller;
    controller.stop = &stop;
    controller.max_nodes = std::numeric_limits<std::uint64_t>::max();
    controller.end_time = start_time + hours(1);

    for (const auto& [fen, movestr] : tests) {
        const libataxx::Position pos{fen};
        const int score =
            search::minimax::minimax(controller, stats, stack, pos, 3);
        REQUIRE(score == MATE_SCORE - 1);
        REQUIRE(stats.seldepth == 3);
        REQUIRE(stats.nodes > 0);
        REQUIRE(stats.tb_hits == 0);
        REQUIRE(stats.tt_hits == 0);
        REQUIRE(stack[0].pv.size() == 1);
        REQUIRE(stack[0].pv.at(0) == libataxx::Move::from_uai(movestr));
    }
}
