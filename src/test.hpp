#ifndef TEST_HPP
#define TEST_HPP

#include <iostream>
#include <libataxx/position.hpp>
#include <sstream>
#include <string>
#include "options.hpp"
#include "protocol.hpp"
#include "search.hpp"

bool test_uai_pos() {
    const std::pair<std::string, std::string> tests[] = {
        {"startpos", "x5o/7/2-1-2/7/2-1-2/7/o5x b"},
        {"startpos moves g2", "x5o/7/2-1-2/7/2-1-2/6x/o5x w"},
        {"startpos moves g2 a1a3", "x5o/7/2-1-2/7/o1-1-2/6x/6x b"},
        {"fen x5o/7/2-1-2/7/2-1-2/7/o5x", "x5o/7/2-1-2/7/2-1-2/7/o5x b"},
        {"fen x5o/7/2-1-2/7/2-1-2/7/o5x w", "x5o/7/2-1-2/7/2-1-2/7/o5x w"},
        {"fen x5o/7/2-1-2/7/2-1-2/7/o5x b", "x5o/7/2-1-2/7/2-1-2/7/o5x b"},
        {"fen x5o/7/2-1-2/7/2-1-2/7/o5x w", "x5o/7/2-1-2/7/2-1-2/7/o5x w"},
        {"fen x5o/7/2-1-2/7/2-1-2/7/o5x moves g2",
         "x5o/7/2-1-2/7/2-1-2/6x/o5x w"},
    };
    for (const auto& [input, fen] : tests) {
        libataxx::Position pos;
        std::stringstream ss(input);
        UAI::position(pos, ss);
        if (pos.get_fen() != fen) {
            return false;
        }
    }
    return true;
}

bool test_uai_moves() {
    const std::string tests[] = {
        "g2 a1a3",
        "",
    };
    for (const auto& moves : tests) {
        std::cout << "Moves ~" << moves << "~" << std::endl;

        libataxx::Position pos1;
        libataxx::Position pos2;

        {
            std::stringstream ss{moves};
            UAI::moves(pos1, ss);
        }

        {
            std::stringstream ss{moves};
            std::string word;
            while (ss >> word) {
                std::cout << "Move: " << word << std::endl;
                const auto move = libataxx::Move::from_uai(word);
                pos2.makemove(move);
            }
        }

        if (pos1.get_fen() != pos2.get_fen()) {
            std::cout << pos1.get_fen() << " vs " << pos2.get_fen()
                      << std::endl;
            return false;
        }
    }
    return true;
}

bool test_options() {
    // Spin
    Options::spins["TestSpin"] = Options::Spin(0, 10, 5);
    if (Options::spins["TestSpin"].get() != 5) {
        return false;
    }
    Options::set("TestSpin", "-2");
    if (Options::spins["TestSpin"].get() != 0) {
        return false;
    }
    Options::set("TestSpin", "12");
    if (Options::spins["TestSpin"].get() != 10) {
        return false;
    }

    // Check
    Options::checks["TestCheck"] = Options::Check(false);
    if (Options::checks["TestCheck"].get() != false) {
        return false;
    }
    Options::set("TestCheck", "true");
    if (Options::checks["TestCheck"].get() != true) {
        return false;
    }

    // String
    Options::strings["TestString"] = Options::String("Test");
    if (Options::strings["TestString"].get() != "Test") {
        return false;
    }
    Options::set("TestString", "Test2");
    if (Options::strings["TestString"].get() != "Test2") {
        return false;
    }

    // Combo
    Options::combos["TestCombo"] = Options::Combo("A", {"A", "B", "C"});
    if (Options::combos["TestCombo"].get() != "A") {
        return false;
    }
    Options::set("TestCombo", "B");
    if (Options::combos["TestCombo"].get() != "B") {
        return false;
    }
    Options::set("TestCombo", "C");
    if (Options::combos["TestCombo"].get() != "C") {
        return false;
    }
    Options::set("TestCombo", "D");
    if (Options::combos["TestCombo"].get() != "C") {
        return false;
    }

    // Spin -- Multiple word name/value
    Options::strings["Test String"] = Options::String("Test Value");
    if (Options::strings["Test String"].get() != "Test Value") {
        return false;
    }

    // Clear options
    Options::spins = {};
    Options::checks = {};
    Options::strings = {};
    Options::combos = {};

    return true;
}

bool test_uai_setoption() {
    Options::strings["Test Name"] = Options::String("Replace");
    std::stringstream ss("name Test Name value Some Value");

    UAI::setoption(ss);

    if (Options::strings["Test Name"].get() != "Some Value") {
        return false;
    }

    // Clear options
    Options::spins = {};
    Options::checks = {};
    Options::strings = {};
    Options::combos = {};

    return true;
}

void test() {
    std::cout << std::boolalpha;
    std::cout << test_options() << " -- Options" << std::endl;
    std::cout << test_uai_pos() << " -- UAI::position" << std::endl;
    std::cout << test_uai_moves() << " -- UAI::moves" << std::endl;
    std::cout << test_uai_setoption() << " -- UAI::setoption" << std::endl;
}

#endif
