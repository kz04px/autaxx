#ifndef SEARCH_NNUE_ACCUMULATOR_HPP
#define SEARCH_NNUE_ACCUMULATOR_HPP

#include <cassert>
#include <libataxx/bitboard.hpp>
#include <libataxx/move.hpp>
#include <libataxx/position.hpp>
#include <libnnue/nnue.hpp>

namespace search::nnue {

class Accumulator {
   public:
    using bias_type = float;
    using weight_type = float;
    using output_type = float;

    static constexpr std::size_t dim_in = 2 * 49;
    static constexpr std::size_t dim_out = 32;

    [[nodiscard]] Accumulator(const libnnue::Vector<bias_type, dim_out> *bias_first,
                              const libnnue::Weights<weight_type, dim_in, dim_out> *weights_first,
                              const libnnue::Vector<bias_type, dim_out> *bias_second,
                              const libnnue::Weights<weight_type, dim_in, dim_out> *weights_second)
        : m_accumulator(bias_first, weights_first, bias_second, weights_second) {
    }

    [[nodiscard]] static std::size_t index(const libataxx::Square sq, const bool flipped) {
        return sq.index() + (flipped ? 49 : 0);
    }

    [[nodiscard]] const auto get() const noexcept {
        return m_accumulator.get();
    }

    void update(const libataxx::Position &pos, const libataxx::Move &move) {
        assert(pos.is_legal_move(move));

        if (move == libataxx::Move::nullmove()) {
            m_accumulator.flip();
            return;
        }

        const auto to_bb = libataxx::Bitboard(move.to());
        const auto neighbours = to_bb.singles();
        const auto captured = neighbours & pos.get_them();
        const bool flippy = pos.get_turn() == libataxx::Side::White;

        // Add piece
        m_accumulator.update_first(index(move.to(), flippy), true);
        m_accumulator.update_second(index(move.to(), !flippy), true);

        if (move.is_double()) {
            // Remove piece
            m_accumulator.update_first(index(move.from(), flippy), false);
            m_accumulator.update_second(index(move.from(), !flippy), false);
        }

        // Flip captured
        for (const auto sq : captured) {
            // Remove piece
            m_accumulator.update_first(index(sq, !flippy), false);
            m_accumulator.update_second(index(sq, flippy), false);

            // Add piece
            m_accumulator.update_first(index(sq, flippy), true);
            m_accumulator.update_second(index(sq, !flippy), true);
        }

        m_accumulator.flip();
    }

    void set_pos(const libataxx::Position &pos) noexcept {
        m_accumulator.clear();

        // Black pieces
        for (const auto sq : pos.get_black()) {
            m_accumulator.update_first(index(sq, false), true);
            m_accumulator.update_second(index(sq, true), true);
        }

        // White pieces
        for (const auto sq : pos.get_white()) {
            m_accumulator.update_first(index(sq, true), true);
            m_accumulator.update_second(index(sq, false), true);
        }

        // Are we flipped?
        m_accumulator.flip(pos.get_turn() == libataxx::Side::White);
    }

   private:
    libnnue::AccumulatorUnshared<bias_type, weight_type, output_type, dim_in, dim_out, libnnue::relu> m_accumulator;
};

}  // namespace search::nnue

#endif
