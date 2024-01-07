//
// Created by ultcrt on 23-11-15.
//

#ifndef ULTRENDERER_INCLUDE_DATA_IMAGE_H_
#define ULTRENDERER_INCLUDE_DATA_IMAGE_H_

#include <vector>
#include "math/Matrix.h"
#include "utils/Proxy.h"

namespace UltRenderer {
    namespace Data {
        /*----------Declaration----------*/
        enum class ImageFormat: std::size_t {
            GRAY = 1, RGB = 3, RGBA = 4
        };

        enum class ImageDirection: std::size_t {
            HORIZONTAL = 1, VERTICAL = 2
        };

        template<ImageFormat FORMAT>
        using PixelProxy = Utils::MatrixProxy<double, static_cast<std::size_t>(FORMAT), 1>;

        // Why not "using Pixel = Math::VectorXD<static_cast<std::size_t>(FORMAT)>;" ?
        // Because compiler cannot infer template parameter from above line
        template<ImageFormat FORMAT>
        class Pixel: public Math::VectorXD<static_cast<std::size_t>(FORMAT)> {
        public:
            // Tips: Inherit all constructor of base class
            using Math::VectorXD<static_cast<std::size_t>(FORMAT)>::VectorXD;

            Pixel(const Math::VectorXD<static_cast<std::size_t>(FORMAT)>& target);
        };

        class Image {
        private:
            std::size_t         _format;
            std::size_t         _width;
            std::size_t         _height;
            std::vector<double> _data;

        public:
            Image(std::size_t w, std::size_t h, ImageFormat format);

            explicit Image(const std::string& filename);

            template<ImageFormat FORMAT>
            Image(std::size_t w, std::size_t h, const Pixel<FORMAT>& filledPixel);

            template<ImageFormat FORMAT>
            void fill(const Pixel<FORMAT>& filledPixel);

            void fill(double val);

            template<ImageFormat FORMAT>
            PixelProxy<FORMAT> at(std::size_t w, std::size_t h);

            template<ImageFormat FORMAT>
            Pixel<FORMAT> at(std::size_t w, std::size_t h) const;

            void save(const std::string& filename);
            [[nodiscard]] Math::Vector2S shape() const;
            [[nodiscard]] std::size_t width() const;
            [[nodiscard]] std::size_t height() const;
            [[nodiscard]] ImageFormat type() const;
            void flip(ImageDirection direction);
        };

        /*----------Definition----------*/
        template<ImageFormat FORMAT>
        Pixel<FORMAT>::Pixel(const Math::VectorXD<static_cast<std::size_t>(FORMAT)>& target): Math::VectorXD<static_cast<std::size_t>(FORMAT)>(target) {}

        template<ImageFormat FORMAT>
        Pixel<FORMAT> Image::at(std::size_t w, std::size_t h) const {
            // Only fill when given pixel has more dimension than image
            assert(_format <= static_cast<std::size_t>(FORMAT));
            assert(w < _width);
            assert(h < _height);

            Pixel<FORMAT> pixel;

            for (std::size_t idx = 0; idx < _format; idx++) {
                pixel[idx] = _data[(h * _width + w) * _format + idx];
            }

            return pixel;
        }

        template<ImageFormat FORMAT>
        PixelProxy<FORMAT> Image::at(std::size_t w, std::size_t h) {
            assert(_format <= static_cast<std::size_t>(FORMAT));
            assert(w < _width);
            assert(h < _height);

            std::array<double*, static_cast<std::size_t>(FORMAT)> componentPtrs;

            for (std::size_t idx = 0; idx < _format; idx++) {
                componentPtrs[idx] = &(_data[(h * _width + w) * _format + idx]);
            }

            return PixelProxy<FORMAT>(componentPtrs);
        }

        template<ImageFormat FORMAT>
        void Image::fill(const Pixel<FORMAT> &filledPixel) {
            assert(_format <= static_cast<std::size_t>(FORMAT));

            // Check all channels of filledPixel is the same or not.
            bool sameChannelValue = true;
            double channelValue = filledPixel[0];
            for (auto iter = filledPixel.begin() + 1; iter != filledPixel.end(); iter++) {
                if (channelValue != *iter) {
                    sameChannelValue = false;
                    break;
                }
            }

            if (sameChannelValue) {
                // All channel are the same, can use std::fill
                fill(filledPixel[0]);
            }
            else {
                for (std::size_t w = 0; w < _width; w++) {
                    for (std::size_t h = 0; h < _height; h++) {
                        for (std::size_t idx = 0; idx < _format; idx++) {
                            _data[(h * _width + w) * _format + idx] = filledPixel[idx];
                        }
                    }
                }
            }
        }

        template<ImageFormat FORMAT>
        Image::Image(std::size_t w, std::size_t h, const Pixel<FORMAT> &filledPixel): _format(static_cast<std::size_t>(FORMAT)), _data(h * w * _format), _width(w), _height(h) {
            fill<FORMAT>(filledPixel);
        }
    } // UltRenderer
} // Data

#endif //ULTRENDERER_INCLUDE_DATA_IMAGE_H_
