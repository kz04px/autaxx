#include "../../autaxx/protocol/common/position.hpp"
#include <catch2/catch.hpp>
#include <libataxx/position.hpp>
#include <sstream>
#include <string>

TEST_CASE("UAI::pos()") {
    const std::pair<std::string, std::string> tests[] = {
        {"startpos", "x5o/7/7/7/7/7/o5x x 0 1"},
        {"startpos moves g2", "x5o/7/7/7/7/6x/o5x o 0 1"},
        {"startpos moves g2 a1a3", "x5o/7/7/7/o6/6x/6x x 1 2"},
        {"fen x5o/7/2-1-2/7/2-1-2/7/o5x", "x5o/7/2-1-2/7/2-1-2/7/o5x x 0 1"},
        {"fen x5o/7/2-1-2/7/2-1-2/7/o5x o", "x5o/7/2-1-2/7/2-1-2/7/o5x o 0 1"},
        {"fen x5o/7/2-1-2/7/2-1-2/7/o5x x", "x5o/7/2-1-2/7/2-1-2/7/o5x x 0 1"},
        {"fen x5o/7/2-1-2/7/2-1-2/7/o5x o", "x5o/7/2-1-2/7/2-1-2/7/o5x o 0 1"},
        {"fen x5o/7/2-1-2/7/2-1-2/7/o5x moves g2", "x5o/7/2-1-2/7/2-1-2/6x/o5x o 0 1"},
    };
    for (const auto& [input, fen] : tests) {
        libataxx::Position pos{"startpos"};
        std::stringstream ss{input};
        common::position(pos, ss);
        REQUIRE(pos.get_fen() == fen);
    }
}
