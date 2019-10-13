#include <catch.hpp>
#include <libataxx/position.hpp>
#include <sstream>
#include <string>
#include "../src/protocol/uai/moves.hpp"

TEST_CASE("UAI::moves()") {
    const std::pair<std::string, std::string> tests[] = {
        {"", "x5o/7/7/7/7/7/o5x x 0 1"},
        {"g2", "x5o/7/7/7/7/6x/o5x o 0 1"},
        {"g2 a1a3", "x5o/7/7/7/o6/6x/6x x 1 2"},
    };
    for (const auto& [moves, fen] : tests) {
        libataxx::Position pos{"startpos"};
        std::stringstream ss{moves};
        UAI::moves(pos, ss);
        REQUIRE(pos.get_fen() == fen);
    }
}
