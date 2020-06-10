#ifndef SEARCH_MCTS_NODE_HPP
#define SEARCH_MCTS_NODE_HPP

#include <cassert>
#include <cmath>
#include <libataxx/move.hpp>
#include <libataxx/position.hpp>
#include <vector>

namespace search::mcts {

class Node {
   public:
    Node(const libataxx::Position &pos)
        : parent_{nullptr},
          move_{},
          reward_{0.0},
          visits_{0},
          children_left_{pos.count_moves()},
          children_{},
          probabilities_{} {
        assert(root());
        assert(!has_parent());
        children_.reserve(children_left_);
        probabilities_.reserve(children_left_);
        calculate_probabilities(pos);
    }

    Node(Node *parent,
         const libataxx::Position &pos,
         const libataxx::Move &move)
        : parent_{parent},
          move_{move},
          reward_{0.0},
          visits_{0},
          children_left_{pos.count_moves()},
          children_{},
          probabilities_{} {
        assert(!root());
        assert(has_parent());
        children_.reserve(children_left_);
        probabilities_.reserve(children_left_);
        calculate_probabilities(pos);
    }

    [[nodiscard]] constexpr auto visited() const noexcept {
        return visits_ > 0;
    }

    [[nodiscard]] constexpr auto reward() const noexcept {
        return reward_;
    }

    [[nodiscard]] auto expanded() const noexcept {
        return !children_.empty();
    }

    [[nodiscard]] constexpr auto expandable() const noexcept {
        return children_left_ > 0;
    }

    [[nodiscard]] constexpr auto terminal() const noexcept {
        return !expandable() && !expanded();
    }

    [[nodiscard]] constexpr auto visits() const noexcept {
        return visits_;
    }

    [[nodiscard]] constexpr auto parent() const noexcept {
        return parent_;
    }

    [[nodiscard]] constexpr bool has_parent() const noexcept {
        return parent() != nullptr;
    }

    [[nodiscard]] auto child_score(const std::size_t idx, const float c) const
        noexcept {
        assert(idx < num_children());

        const auto &child = children_[idx];

        if (child.visits() == 0) {
            return 100000.0f;
        }

        const float q = child.reward() / child.visits();
        const float u = c * probabilities_[idx] * std::sqrt(visits() - 1) /
                        (child.visits() + 1);
        return q + u;
    }

    [[nodiscard]] auto most_visited_child() const noexcept {
        std::size_t best_idx = 0;

        for (std::size_t i = 1; i < num_children(); ++i) {
            if (children_[i].visits() > children_[best_idx].visits()) {
                best_idx = i;
            }
        }

        assert(best_idx < num_children());
        return best_idx;
    }

    [[nodiscard]] auto best_scoring_child(const float c = 1.0) const noexcept {
        std::size_t best_idx = 0;
        float best_score = std::numeric_limits<float>::lowest();

        for (std::size_t i = 0; i < num_children(); ++i) {
            const auto score = child_score(i, c);
            if (score > best_score) {
                best_score = score;
                best_idx = i;
            }
        }

        assert(best_idx < num_children());
        return best_idx;
    }

    [[nodiscard]] auto stable() const noexcept {
        return most_visited_child() == best_scoring_child(0.0f);
    }

    [[nodiscard]] std::size_t num_children() const noexcept {
        return children_.size();
    }

    [[nodiscard]] auto child(const std::size_t idx) noexcept {
        assert(idx < num_children());
        return &children_[idx];
    }

    [[nodiscard]] constexpr auto move() const noexcept {
        return move_;
    }

    [[nodiscard]] Node *expand(const libataxx::Position &pos) noexcept {
        assert(!terminal());
        assert(expandable());

        libataxx::Move moves[libataxx::max_moves];
        const int num_moves = pos.legal_moves(moves);
        assert(num_children() < num_moves);
        assert(num_moves <= children_.capacity());
        assert(pos.legal_move(moves[num_children()]));
        auto npos = pos;
        npos.makemove(moves[num_children()]);
        children_.emplace_back(this, npos, moves[num_children()]);
        children_left_--;
        return &children_.back();
    }

    [[nodiscard]] std::vector<libataxx::Move> get_pv(
        const int max_length = 8) const noexcept {
        std::vector<libataxx::Move> moves;
        const Node *n = this;
        while (n->expanded()) {
            const int idx = n->most_visited_child();
            assert(idx < n->num_children());
            n = &n->children_[idx];
            assert(n);
            moves.push_back(n->move());
            if (moves.size() >= max_length) {
                break;
            }
        }
        return moves;
    }

    [[nodiscard]] constexpr const std::vector<Node> &children() const noexcept {
        return children_;
    }

    [[nodiscard]] constexpr bool root() const noexcept {
        return !has_parent();
    }

    constexpr void add_visit() noexcept {
        visits_++;
    }

    constexpr void add_reward(const float n) noexcept {
        reward_ += n;
    }

   private:
    void calculate_probabilities(const libataxx::Position &pos) {
        if (terminal()) {
            return;
        }

        libataxx::Move moves[libataxx::max_moves];
        const int num_moves = pos.legal_moves(moves);
        float total = 0.0f;

        // Probabilities
        for (int i = 0; i < num_moves; ++i) {
            const float p = pos.count_captures(moves[i]) + moves[i].is_single();
            probabilities_.push_back(p);
            total += p;
        }

        // Softmax
        for (auto &prob : probabilities_) {
            if (total == 0.0) {
                prob = 1.0f / num_moves;
            } else {
                prob = prob / total;
            }
        }

#ifndef NDEBUG
        float ntotal = 0.0f;
        for (auto &prob : probabilities_) {
            ntotal += prob;
        }
        assert(std::abs(ntotal - 1.0f) <= 0.001f);
#endif
    }

    Node *parent_;
    libataxx::Move move_;
    float reward_;
    int visits_;
    int children_left_;
    std::vector<Node> children_;
    std::vector<float> probabilities_;
};

}  // namespace search::mcts

#endif
