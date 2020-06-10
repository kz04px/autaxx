#include <catch2/catch.hpp>
#include <cstdint>
#include <libataxx/move.hpp>
#include <libataxx/position.hpp>
#include <string>
#include "../src/search/tt.hpp"

struct Entry {
    std::uint64_t hash;
    std::uint64_t nodes;
    std::uint8_t depth;
};

search::TT<Entry> tt{16};

std::uint64_t ttperft(const libataxx::Position &pos, const std::uint8_t depth) {
    if (depth == 1) {
        return pos.count_moves();
    } else if (depth == 0) {
        return 1;
    }

    const auto entry = tt.poll(pos.hash());
    if (entry.hash == pos.hash() && entry.depth == depth) {
        return entry.nodes;
    }

    libataxx::Move moves[libataxx::max_moves];
    const int num_moves = pos.legal_moves(moves);

    std::uint64_t nodes = 0;

    for (int i = 0; i < num_moves; ++i) {
        auto npos = pos;
        npos.makemove(moves[i]);
        nodes += ttperft(npos, depth - 1);
    }

    tt.add(pos.hash(), Entry{pos.hash(), nodes, depth});

    return nodes;
}

TEST_CASE("hashtable") {
    const std::string fens[] = {
        "startpos",
        "x5o/7/2-1-2/7/2-1-2/7/o5x x 0 1",
    };

    for (const auto &fen : fens) {
        libataxx::Position pos{fen};
        tt.clear();
        for (int i = 0; i <= 5; ++i) {
            REQUIRE(pos.perft(i) == ttperft(pos, i));
        }
    }
}
