#ifndef SEARCH_ALPHABETA_HPP
#define SEARCH_ALPHABETA_HPP

#include <libataxx/move.hpp>
#include <libataxx/position.hpp>
#include "../../utils.hpp"
#include "../pv.hpp"
#include "../search.hpp"
#include "../tt.hpp"
#include "ttentry.hpp"

namespace search {

namespace alphabeta {

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

class Alphabeta : public Search {
   public:
    struct Stack {
        int ply;
        PV pv;
        libataxx::Move killer;
        bool nullmove;
    };

    Alphabeta(const unsigned int mb) : tt_{mb} {
    }

    void go(const libataxx::Position pos, const Settings &settings) override {
        stop();
        search_thread_ = std::thread(&Alphabeta::root, this, pos, settings);
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

   private:
    [[nodiscard]] static int eval(const libataxx::Position &pos) noexcept;

    void root(const libataxx::Position pos, const Settings &settings) noexcept;

    [[nodiscard]] int alphabeta(Stack *stack,
                                const libataxx::Position &pos,
                                int alpha,
                                int beta,
                                int depth);

    Stack stack_[max_depth + 1];
    TT<TTEntry> tt_;
};

}  // namespace alphabeta

}  // namespace search

#endif
