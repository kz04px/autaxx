#ifndef SEARCH_NNUE_NETWORK_HPP
#define SEARCH_NNUE_NETWORK_HPP

#include <libataxx/position.hpp>
#include <libnnue/nnue.hpp>
#include "accumulator.hpp"

namespace search::nnue {

class Network {
   public:
    [[nodiscard]] Network() = default;

    [[nodiscard]] Network(const std::string &path) {
        load_weights(path);
    }

    void load_weights(const std::string &path) {
        std::ifstream file(path, std::ios::binary);

        if (!file) {
            throw "Could not open file";
        }

        // Read feature layer
        m_weights_white.load_file(file);
        m_bias_white.load_file(file);
        m_weights_black.load_file(file);
        m_bias_black.load_file(file);

        // Read FNN
        m_weights_1.load_file(file);
        m_bias_1.load_file(file);

        m_weights_2.load_file(file);
        m_bias_2.load_file(file);

        m_weights_3.load_file(file);
        m_bias_3.load_file(file);
    }

    [[nodiscard]] Accumulator new_accumulator() const noexcept {
        return Accumulator(&m_bias_black, &m_weights_black, &m_bias_white, &m_weights_white);
    }

    [[nodiscard]] float run(const Accumulator &accumulator) const noexcept {
        // Feature layer
        const auto a = accumulator.get();

        // FNN - Layer 1 - 64x32 - ReLU
        const auto b = libnnue::multiply<float>(a, m_weights_1).add(m_bias_1).apply(libnnue::relu<float>);

        // FNN - Layer 2 - 32x32 - ReLU
        const auto c = libnnue::multiply<float>(b, m_weights_2).add(m_bias_2).apply(libnnue::relu<float>);

        // FNN - Layer 3 - 64x1 - None
        const auto in3 = libnnue::combine(b, c);
        const auto d = libnnue::multiply<float>(in3, m_weights_3).add(m_bias_3);

        return d.at(0);
    }

    [[nodiscard]] auto run(const libataxx::Position &pos) const noexcept {
        auto accumulator = new_accumulator();
        accumulator.set_pos(pos);
        return run(accumulator);
    }

   private:
    // Feature layer
    libnnue::Weights<float, 2 * 49, 32> m_weights_black;
    libnnue::Vector<float, 32> m_bias_black;
    libnnue::Weights<float, 2 * 49, 32> m_weights_white;
    libnnue::Vector<float, 32> m_bias_white;
    // Layer 1
    libnnue::Weights<float, 64, 32> m_weights_1;
    libnnue::Vector<float, 32> m_bias_1;
    // Layer 2
    libnnue::Weights<float, 32, 32> m_weights_2;
    libnnue::Vector<float, 32> m_bias_2;
    // Layer 3
    libnnue::Weights<float, 64, 1> m_weights_3;
    libnnue::Vector<float, 1> m_bias_3;
};

}  // namespace search::nnue

#endif
