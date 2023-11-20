//
// Created by ultcrt on 23-11-15.
//

#include "../../include/data/Image.h"

namespace UltRenderer {
    namespace Data {
        template<>
        std::array<std::size_t, GRAY> Image<GRAY>::get(std::size_t w, std::size_t h) {
            std::array<std::size_t, GRAY> color{};

            color[0] = _data[(_width * h + w) * GRAY + 2];

            return color;
        }

        template<>
        std::array<std::size_t, RGB> Image<RGB>::get(std::size_t w, std::size_t h) {
            std::size_t offset = (_width * h + w) * RGB;

            return UltRenderer::Utils::ConvertBGR2RGB({_data[offset + 0], _data[offset + 1], _data[offset + 2]});
        }

        template<>
        std::array<std::size_t, RGBA> Image<RGBA>::get(std::size_t w, std::size_t h) {
            std::size_t offset = (_width * h + w) * RGBA;

            return UltRenderer::Utils::ConvertBGRA2RGBA({_data[offset + 0], _data[offset + 1], _data[offset + 2], _data[offset + 3]});;
        }

        template<>
        void Image<GRAY>::set(std::size_t w, std::size_t h, const std::array<std::size_t, GRAY>& gray) {
            _data[(_width * h + w) * GRAY] = gray[0];
        }

        template<>
        void Image<RGB>::set(std::size_t w, std::size_t h, const std::array<std::size_t, RGB>& rgb) {
            const std::array<std::size_t, RGB> bgr = UltRenderer::Utils::ConvertRGB2BGR(rgb);
            for (std::size_t idx = 0; idx < RGB; idx++) {
                _data[(_width * h + w) * RGB + idx] = bgr[idx];
            }
        }

        template<>
        void Image<RGBA>::set(std::size_t w, std::size_t h, const std::array<std::size_t, RGBA>& rgba) {
            const std::array<std::size_t, RGBA> bgra = UltRenderer::Utils::ConvertRGBA2BGRA(rgba);
            for (std::size_t idx = 0; idx < RGBA; idx++) {
                _data[(_width * h + w) * RGBA + idx] = bgra[idx];
            }
        }
    } // UltRenderer
} // Data