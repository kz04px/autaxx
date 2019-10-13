#ifndef SEARCH_MINIMAX_HPP
#define SEARCH_MINIMAX_HPP

#include <libataxx/move.hpp>
#include <libataxx/position.hpp>
#include "../../utils.hpp"
#include "../pv.hpp"
#include "../search.hpp"

namespace search {

namespace minimax {

constexpr int mate_score = 10000;
constexpr int max_depth = 128;

class Minimax : public Search {
   public:
    struct Stack {
        int ply;
        PV pv;
    };

    void go(const libataxx::Position pos, const Settings &settings) override {
        stop();
        search_thread_ = std::thread(&Minimax::root, this, pos, settings);
    }

   private:
    void root(const libataxx::Position pos, const Settings &settings) noexcept;

    [[nodiscard]] static int eval(const libataxx::Position &pos) noexcept;

    [[nodiscard]] int minimax(Stack *stack,
                              const libataxx::Position &pos,
                              int depth);

    Stack stack_[max_depth + 1];
};

}  // namespace minimax

}  // namespace search

#endif
