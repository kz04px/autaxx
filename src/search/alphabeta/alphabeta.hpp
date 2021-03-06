#ifndef SEARCH_ALPHABETA_HPP
#define SEARCH_ALPHABETA_HPP

#include <libataxx/move.hpp>
#include <libataxx/position.hpp>
#include "../../utils.hpp"
#include "../pv.hpp"
#include "../search.hpp"

namespace search::alphabeta {

constexpr int mate_score = 10000;
constexpr int max_depth = 128;

class Alphabeta : public Search {
   public:
    struct Stack {
        int ply;
        PV pv;
    };

    void go(const libataxx::Position pos, const Settings &settings) override {
        stop();
        search_thread_ = std::thread(&Alphabeta::root, this, pos, settings);
    }

   private:
    void root(const libataxx::Position pos, const Settings &settings) noexcept;

    [[nodiscard]] static int eval(const libataxx::Position &pos) noexcept;

    [[nodiscard]] int search(Stack *stack, const libataxx::Position &pos, int alpha, const int beta, int depth);

    Stack stack_[max_depth + 1];
};

}  // namespace search::alphabeta

#endif
