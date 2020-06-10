#include <catch2/catch.hpp>
#include <cstdint>
#include <libataxx/move.hpp>
#include <libataxx/position.hpp>
#include <vector>
#include "../src/search/alphabeta/sorter.hpp"
#include "../src/utils.hpp"

std::uint64_t perft(const libataxx::Position &pos, const int depth) {
    if (depth == 0) {
        return 1;
    }

    libataxx::Move ttmove;
    libataxx::Move killer;

    {
        libataxx::Move moves[libataxx::max_moves];
        const int num_moves = pos.legal_moves(moves);

        ttmove = moves[utils::rand_u32(0, num_moves)];
        killer = moves[utils::rand_u32(0, num_moves)];

        // ttmove and killer should occasionally be illegal moves
        const auto a = utils::rand_u32(0, 19);
        if (a == 0) {
            ttmove = libataxx::Move::nomove();
        } else if (a == 1) {
            ttmove = libataxx::Move::nullmove();
        } else if (a == 2) {
            const auto file = libataxx::File(utils::rand_u32(0, 6));
            const auto rank = libataxx::Rank(utils::rand_u32(0, 6));
            const auto sq = libataxx::Square(file, rank);
            const auto move = libataxx::Move(sq);
            ttmove = move;
        }

        const auto b = utils::rand_u32(0, 19);
        if (b == 0) {
            killer = libataxx::Move::nomove();
        } else if (b == 1) {
            killer = libataxx::Move::nullmove();
        } else if (b == 2) {
            const auto file = libataxx::File(utils::rand_u32(0, 6));
            const auto rank = libataxx::Rank(utils::rand_u32(0, 6));
            const auto sq = libataxx::Square(file, rank);
            const auto move = libataxx::Move(sq);
            killer = move;
        }
    }

    auto sorter = search::alphabeta::Sorter{pos, ttmove, killer};
    libataxx::Move move;
    std::uint64_t nodes = 0;

    const bool tt_legal = pos.legal_move(ttmove);
    const bool killer_legal = pos.legal_move(killer);

    const bool tt_first = tt_legal;
    const bool killer_first =
        (tt_first && ttmove == killer) || (!tt_first && killer_legal);
    const bool killer_second = tt_first && killer_legal && ttmove != killer;

    int i = 0;
    while (sorter.next(move)) {
        switch (i) {
            case 0:
                REQUIRE((!tt_first || move == ttmove));
                REQUIRE((!killer_first || move == killer));
                break;
            case 1:
                REQUIRE((!killer_second || move == killer));
                break;
            default:
                REQUIRE(move != ttmove);
                REQUIRE(move != killer);
                break;
        }

        libataxx::Position npos = pos;
        npos.makemove(move);
        nodes += perft(npos, depth - 1);
        i++;
    }

    REQUIRE(i == pos.count_moves());

    return nodes;
}

TEST_CASE("Alphabeta sorter") {
    const std::pair<std::string, std::vector<std::uint64_t>> tests[] = {
        {"7/7/7/7/7/7/7 x 0 1", {1, 0, 0, 0, 0}},
        {"7/7/7/7/7/7/7 o 0 1", {1, 0, 0, 0, 0}},
        {"x5o/7/7/7/7/7/o5x x 0 1", {1, 16, 256, 6460, 155888, 4752668}},
        {"x5o/7/7/7/7/7/o5x o 0 1", {1, 16, 256, 6460, 155888, 4752668}},
        {"x5o/7/2-1-2/7/2-1-2/7/o5x x 0 1", {1, 14, 196, 4184, 86528, 2266352}},
        {"x5o/7/2-1-2/7/2-1-2/7/o5x o 0 1", {1, 14, 196, 4184, 86528, 2266352}},
        {"7/7/7/7/ooooooo/ooooooo/xxxxxxx x 0 1", {1, 1, 75, 249, 14270}},
        {"7/7/7/7/ooooooo/ooooooo/xxxxxxx o 0 1", {1, 75, 249, 14270, 452980}},
        {"7/7/7/2x1o2/7/7/7 x 0 1", {1, 23, 419, 7887, 168317, 4266992}},
        {"7/7/7/2x1o2/7/7/7 o 0 1", {1, 23, 419, 7887, 168317, 4266992}},
    };

    for (const auto &[fen, nodes] : tests) {
        libataxx::Position pos{fen};
        for (int i = 0; i < nodes.size(); ++i) {
            REQUIRE(nodes[i] == perft(pos, i));
        }
    }
}
