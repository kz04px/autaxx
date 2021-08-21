#ifndef SEARCH_NNUE_HPP
#define SEARCH_NNUE_HPP

#include <libataxx/move.hpp>
#include <libataxx/position.hpp>
#include "../../utils.hpp"
#include "../pv.hpp"
#include "../search.hpp"
#include "../tt.hpp"
#include "network.hpp"
#include "ttentry.hpp"

namespace search::nnue {

constexpr int mate_score = 10000;
constexpr int max_depth = 128;

constexpr int eval_to_tt(const int eval, const int ply) {
    if (eval > mate_score - max_depth) {
        return eval + ply;
    }
    if (eval < -mate_score + max_depth) {
        return eval - ply;
    }
    return eval;
}

constexpr int eval_from_tt(const int eval, const int ply) {
    if (eval > mate_score - max_depth) {
        return eval - ply;
    }
    if (eval < -mate_score + max_depth) {
        return eval + ply;
    }
    return eval;
}

class NNUE : public Search {
   public:
    struct Stack {
        int ply;
        PV pv;
        libataxx::Move killer;
        bool nullmove;
    };

    NNUE(const std::string &nnue_path, const unsigned int mb)
        : m_network{}, m_accumulator{m_network.new_accumulator()}, stack_{}, tt_{mb} {
        m_network.load_weights(nnue_path);
    }

    void go(const libataxx::Position pos, const Settings &settings) override {
        stop();
        search_thread_ = std::thread(&NNUE::root, this, pos, settings);
    }

    void clear() noexcept override {
        tt_.clear();
        for (int i = 0; i < max_depth + 1; ++i) {
            stack_[i].ply = i;
            stack_[i].pv.clear();
            stack_[i].killer = libataxx::Move::nomove();
            stack_[i].nullmove = true;
        }
    }

    [[nodiscard]] int eval() const noexcept;

    [[nodiscard]] int eval(const libataxx::Position &pos) const noexcept;

   private:
    void root(const libataxx::Position pos, const Settings &settings) noexcept;

    [[nodiscard]] int search(Stack *stack, const libataxx::Position &pos, int alpha, int beta, int depth);

    Network m_network;
    Accumulator m_accumulator;
    Stack stack_[max_depth + 1];
    TT<TTEntry> tt_;
};

}  // namespace search::nnue

#endif
