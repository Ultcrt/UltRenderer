//
// Created by ultcrt on 23-11-20.
//

#include "utils/Color.h"

namespace UltRenderer {
    namespace Utils {
        std::array<std::size_t, 3> ConvertRGB2BGR(const std::array<std::size_t, 3>& color) {
            std::array<std::size_t, 3> result {color[2], color[1], color[0]};
            return result;
        }

        std::array<std::size_t, 3> ConvertBGR2RGB(const std::array<std::size_t, 3>& color) {
            return ConvertRGB2BGR(color);
        }

        std::array<std::size_t, 4> ConvertRGBA2BGRA(const std::array<std::size_t, 4>& color) {
            std::array<std::size_t, 4> result = {color[2], color[1], color[0], color[3]};;
            return result;
        }

        std::array<std::size_t, 4> ConvertBGRA2RGBA(const std::array<std::size_t, 4>& color) {
            return ConvertRGBA2BGRA(color);
        }
    } // UltRenderer
} // Utils