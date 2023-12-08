//
// Created by ultcrt on 23-12-6.
//

#include <sstream>
#include "utils/String.h"

std::vector<std::string> UltRenderer::Utils::String::Split(const std::string &str, char delimiter) {
    std::istringstream lineStream(str);
    std::vector<std::string> split;
    std::string word;
    while (std::getline(lineStream, word, delimiter)) {
        split.emplace_back(word);
    }
    return split;
}