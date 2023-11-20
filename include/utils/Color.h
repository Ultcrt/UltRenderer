//
// Created by ultcrt on 23-11-20.
//

#ifndef ULTRENDERER_COLOR_H
#define ULTRENDERER_COLOR_H

#include <array>

namespace UltRenderer {
    namespace Utils {
        std::array<std::size_t, 3> ConvertRGB2BGR(const std::array<std::size_t, 3>& color);
        std::array<std::size_t, 3> ConvertBGR2RGB(const std::array<std::size_t, 3>& color);
        std::array<std::size_t, 4> ConvertRGBA2BGRA(const std::array<std::size_t, 4>& color);
        std::array<std::size_t, 4> ConvertBGRA2RGBA(const std::array<std::size_t, 4>& color);
    } // UltRenderer
} // Utils

#endif //ULTRENDERER_COLOR_H
