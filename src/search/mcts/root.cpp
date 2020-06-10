#include <cassert>
#include <cmath>
#include <iostream>
#include "eval.hpp"
#include "mcts.hpp"
#include "node.hpp"

using namespace std::chrono;

namespace search::mcts {

float sigmoid(const float score, const float k = 1.13) {
    return 1.0f / (1.0f + std::pow(10.0f, -k * score / 400.0f));
}

Node *tree_policy(Node *n, libataxx::Position &pos) {
    assert(n);

    while (!n->terminal()) {
        if (n->expandable()) {
            n = n->expand(pos);
            assert(pos.legal_move(n->move()));
            pos.makemove(n->move());
            return n;
        } else {
            const int idx = n->best_scoring_child();
            assert(pos.legal_move(n->child(idx)->move()));
            pos.makemove(n->child(idx)->move());
            n = n->child(idx);
        }
    }
    return n;
}

float default_policy(const libataxx::Position &pos) {
    float score = 0.5f;

    switch (pos.result()) {
        case libataxx::Result::BlackWin:
        case libataxx::Result::WhiteWin:
            score = 0.0f;
            break;
        case libataxx::Result::Draw:
            score = 0.5f;
            break;
        case libataxx::Result::None:
            score = sigmoid(0.1 * eval(pos));
            break;
        default:
            abort();
    }

    return 1.0f - score;
}

void backup_negamax(Node *n, float delta) {
    assert(n);
    assert(!n->expanded());
    assert(0.0 <= delta && delta <= 1.0);

    while (n) {
        n->add_visit();
        n->add_reward(delta);
        delta = 1.0f - delta;
        n = n->parent();
    }
}

bool legal_pv(const libataxx::Position &pos,
              const std::vector<libataxx::Move> &moves) {
    auto npos = pos;
    int ply = 0;
    for (const auto &move : moves) {
        if (!npos.legal_move(move)) {
            break;
        } else {
            npos.makemove(move);
            ply++;
        }
    }
    return moves.size() == ply;
}

void print(const Node *n, const int depth) {
    if (depth == 3) {
        return;
    }

    for (int i = 0; i < depth; ++i) {
        std::cout << "    ";
    }

    std::cout << n->move();
    std::cout << " n=" << n->visits();
    std::cout << " q=" << n->reward();
    std::cout << " wr=" << n->reward() / n->visits();
    std::cout << " t=" << n->terminal();
    std::cout << std::endl;
    for (const auto &child : n->children()) {
        print(&child, depth + 1);
    }
}

bool verify(const Node *n) {
    assert(n);

    // Reward
    if (n->reward() < 0.0 || n->reward() > n->visits()) {
        std::cout << "Reward error" << std::endl;
        return false;
    }

    // Visits
    if (!n->visited() || n->visits() < 1) {
        std::cout << "Visit error" << std::endl;
        return false;
    }

    // Root
    if (n->root()) {
        // Root has no parent
        if (n->has_parent()) {
            std::cout << "Root parent" << std::endl;
            return false;
        }

        // Root has no move
        if (n->move()) {
            std::cout << "Root move" << std::endl;
            return false;
        }
    } else {
        // Non-root must have a parent
        if (!n->has_parent()) {
            std::cout << "Non-root parent" << std::endl;
            return false;
        }

        // Non-root must have a move
        if (!n->move()) {
            std::cout << "Non-root move" << std::endl;
            return false;
        }
    }

    // Terminal
    if (n->terminal()) {
        // Stuff
        if (n->expanded() || n->expandable() || n->num_children() > 0) {
            std::cout << "Terminal - Stuff" << std::endl;
            return false;
        }

        // Winrate
        const float winrate = n->reward() / n->visits();
        if ((0.001 <= winrate && winrate <= 0.499) ||
            (0.501 <= winrate && winrate <= 0.999)) {
            std::cout << "Terminal - Winrate" << std::endl;
            return false;
        }
    }

    // Expanded
    if (n->expanded()) {
        // Stuff
        if (n->num_children() == 0 || n->terminal()) {
            std::cout << "Expanded - Stuff" << std::endl;
            return false;
        }

        // Most (known) children possible
        if (n->num_children() > 194) {
            std::cout << "Expanded - Too many children" << std::endl;
            return false;
        }

        // Visits
        int child_visits = 0;
        for (const auto &child : n->children()) {
            child_visits += child.visits();
        }
        if (n->visits() - !n->root() != child_visits) {
            std::cout << "Expanded - Visits mismatch" << std::endl;
            return false;
        }
    }

    // Recurse
    for (const auto &child : n->children()) {
        if (!verify(&child)) {
            return false;
        }
    }

    return true;
}

void MCTS::root(const libataxx::Position pos,
                const Settings &settings) noexcept {
    const auto start_time = high_resolution_clock::now();
    controller_.max_nodes = std::numeric_limits<std::uint64_t>::max();
    controller_.end_time = start_time + hours(1);

    // Time management
    switch (settings.type) {
        case Type::Time: {
            int search_time = 0;

            // Calculate time usage
            if (pos.turn() == libataxx::Side::Black) {
                search_time = settings.btime / 30;
            } else {
                search_time = settings.wtime / 30;
            }

            // Minimum time
            if (search_time < 1) {
                search_time = 1;
            }
            assert(search_time > 0);

            controller_.end_time = start_time + milliseconds(search_time);
            break;
        }
        case Type::Depth:
            break;
        case Type::Nodes:
            controller_.max_nodes = settings.nodes;
            break;
        case Type::Movetime:
            controller_.end_time = start_time + milliseconds(settings.movetime);
            break;
        case Type::Infinite:
            break;
        default:
            break;
    }

    Node root{pos};

    while (true) {
        auto npos = pos;
        Node *selection = tree_policy(&root, npos);
        const auto reward = default_policy(npos);
        backup_negamax(selection, reward);

        stats_.nodes++;

        assert(legal_pv(pos, root.get_pv()));
        assert(verify(&root));

        if (stats_.nodes % 1000 == 0) {
            const auto pv = root.get_pv();

            std::cout << "info";
            std::cout << " nodes " << stats_.nodes;
            if (!pv.empty()) {
                std::cout << " pv";
                for (const auto &move : pv) {
                    std::cout << " " << move;
                }
            }
            std::cout << "\n";
        }

        if (controller_.stop || stats_.nodes >= controller_.max_nodes) {
            break;
        }

        if (stats_.nodes % 1024 == 0 &&
            high_resolution_clock::now() > controller_.end_time) {
            break;
        }
    }

    const auto pv = root.get_pv();
    if (!pv.empty()) {
        std::cout << "bestmove " << pv[0] << std::endl;
    } else {
        std::cout << "0000" << std::endl;
    }
}

}  // namespace search::mcts
