#ifndef SEARCH_ALPHABETA_HPP
#define SEARCH_ALPHABETA_HPP

#include <libataxx/move.hpp>
#include <libataxx/position.hpp>
#include "../../utils.hpp"
#include "../pv.hpp"
#include "../search.hpp"

namespace search {

namespace alphabeta {

constexpr int mate_score = 10000;
constexpr int max_depth = 128;

class Alphabeta : public Search {
   public:
    struct Stack {
        int ply;
        PV pv;
        libataxx::Move killer;
        bool nullmove;
    };

    void go(const libataxx::Position pos, const Settings &settings) override {
        stop();
        search_thread_ = std::thread(&Alphabeta::root, this, pos, settings);
    }

   private:
    [[nodiscard]] static int eval(const libataxx::Position &pos) noexcept;

    void root(const libataxx::Position pos, const Settings &settings) noexcept;

    [[nodiscard]] int alphabeta(Stack *stack,
                                const libataxx::Position &pos,
                                int alpha,
                                const int beta,
                                int depth);

    Stack stack_[max_depth + 1];
};

}  // namespace alphabeta

}  // namespace search

#endif
