#include "setoption.hpp"
#include <string>
#include "../../options.hpp"

namespace UAI {

// Set an option
void setoption(std::stringstream &stream) {
    std::string word = "";

    stream >> word;
    if (word != "name") {
        return;
    }

    // Collect option name
    std::string name = "";
    while (stream >> word && word != "value") {
        if (name != "") {
            name += " ";
        }
        name += word;
    }

    // Collect option value
    std::string value = "";
    while (stream >> word) {
        if (value != "") {
            value += " ";
        }
        value += word;
    }

    if (name != "" && value != "") {
        Options::set(name, value);
    }
}

}  // namespace UAI
