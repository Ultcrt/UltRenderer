//
// Created by ultcrt on 23-12-6.
//

#ifndef ULTRENDERER_STRING_H
#define ULTRENDERER_STRING_H

#include <vector>
#include <string>

namespace UltRenderer {
    namespace Utils {
        namespace String {
            std::vector<std::string> Split(const std::string& str, char delimiter = ' ');
        }
    } // UltRenderer
} // Rendering

#endif //ULTRENDERER_STRING_H
