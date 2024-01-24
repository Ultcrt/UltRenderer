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

        enum class NormalMapType: std::size_t {
            CARTESIAN = 0, DARBOUX = 1
        };

        enum class FilterType: std::size_t {
            NEAREST, LINEAR
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

            template<ImageFormat TARGET>
            Pixel<TARGET> to() const;
        };

        class Image {
        private:
            std::size_t         _format;
            std::size_t         _width;
            std::size_t         _height;
            std::vector<double> _data;

        public:
            FilterType filterType;

            Image(std::size_t w, std::size_t h, ImageFormat format, FilterType filterType = FilterType::NEAREST);

            explicit Image(const std::string& filename, FilterType filterType = FilterType::NEAREST);

            template<ImageFormat FORMAT>
            Image(std::size_t w, std::size_t h, const Pixel<FORMAT>& filledPixel, FilterType filterType = FilterType::NEAREST);

            template<ImageFormat FORMAT>
            void fill(const Pixel<FORMAT>& filledPixel);

            void fill(double val);

            template<ImageFormat FORMAT>
            PixelProxy<FORMAT> at(std::size_t w, std::size_t h);

            template<ImageFormat FORMAT>
            const Pixel<FORMAT> at(std::size_t w, std::size_t h) const;

            template<ImageFormat FORMAT>
            const Pixel<FORMAT> get(double wRatio, double hRatio) const;

            template<ImageFormat FORMAT>
            PixelProxy<FORMAT> at(const Math::Vector2S& pos);

            template<ImageFormat FORMAT>
            const Pixel<FORMAT> at(const Math::Vector2S& pos) const;

            template<ImageFormat FORMAT>
            const Pixel<FORMAT> get(const Math::Vector2D& pos) const;

            void save(const std::string& filename);
            [[nodiscard]] Math::Vector2S shape() const;
            [[nodiscard]] std::size_t width() const;
            [[nodiscard]] std::size_t height() const;
            [[nodiscard]] ImageFormat type() const;
            void flip(ImageDirection direction);
        };

        /*----------Definition----------*/
        template<ImageFormat FORMAT>
        template<ImageFormat TARGET>
        Pixel<TARGET> Pixel<FORMAT>::to() const {
            if constexpr (FORMAT == TARGET) {
                return *this;
            }
            else if constexpr (FORMAT == ImageFormat::GRAY && TARGET == ImageFormat::RGB) {
                return Pixel<TARGET>{1, 1, 1} * this->x();
            }
            else if constexpr (FORMAT == ImageFormat::GRAY && TARGET == ImageFormat::RGBA) {
                return Pixel<TARGET>{this->x(), this->x(), this->x(), 1};
            }
            else if constexpr (FORMAT == ImageFormat::RGB && TARGET == ImageFormat::GRAY) {
                return Pixel<TARGET>{0.299 * this->x() + 0.587 * this->y() + 0.114 * this->z()};
            }
            else if constexpr (FORMAT == ImageFormat::RGB && TARGET == ImageFormat::RGBA) {
                return Pixel<TARGET>{this->x(), this->y(), this->z(), 1};
            }
            else if constexpr (FORMAT == ImageFormat::RGBA && TARGET == ImageFormat::GRAY) {
                Pixel<ImageFormat::RGB> rgb = to<ImageFormat::RGB>();
                return rgb.to<TARGET>();
            }
            else if constexpr (FORMAT == ImageFormat::RGBA && TARGET == ImageFormat::RGB) {
                return Pixel<TARGET>{this->x(), this->y(), this->z()};
            }
        }

        template<ImageFormat FORMAT>
        Pixel<FORMAT>::Pixel(const Math::VectorXD<static_cast<std::size_t>(FORMAT)>& target): Math::VectorXD<static_cast<std::size_t>(FORMAT)>(target) {}

        template<ImageFormat FORMAT>
        const Pixel<FORMAT> Image::at(std::size_t w, std::size_t h) const {
            // Only fill when given pixel has more dimension than image
            assert(static_cast<std::size_t>(FORMAT) <= _format);
            assert(w < _width);
            assert(h < _height);

            Pixel<FORMAT> pixel;

            for (std::size_t idx = 0; idx < static_cast<std::size_t>(FORMAT); idx++) {
                pixel[idx] = _data[(h * _width + w) * _format + idx];
            }

            return pixel;
        }

        template<ImageFormat FORMAT>
        PixelProxy<FORMAT> Image::at(std::size_t w, std::size_t h) {
            assert(static_cast<std::size_t>(FORMAT) <= _format);
            assert(w < _width);
            assert(h < _height);

            std::array<double*, static_cast<std::size_t>(FORMAT)> componentPtrs;

            for (std::size_t idx = 0; idx < static_cast<std::size_t>(FORMAT); idx++) {
                componentPtrs[idx] = &(_data[(h * _width + w) * _format + idx]);
            }

            return PixelProxy<FORMAT>(componentPtrs);
        }

        template<ImageFormat FORMAT>
        const Pixel<FORMAT> Image::get(double wRatio, double hRatio) const {

            switch (filterType) {
                case FilterType::NEAREST: {
                    auto width = static_cast<std::size_t>(std::lround(wRatio * static_cast<double>(_width)));
                    auto height = static_cast<std::size_t>(std::lround(hRatio * static_cast<double>(_height)));
                    return at<FORMAT>(width, height);
                }
                case FilterType::LINEAR: {
                    // TODO: (0.5, 0.5) should be the proper pixel center
                    auto width = wRatio * static_cast<double>(_width);
                    auto height = hRatio * static_cast<double>(_height);

                    std::size_t w0 = 0;
                    std::size_t w1 = 0;
                    std::size_t h0 = 0;
                    std::size_t h1 = 0;
                    double ww = 0;
                    double hw = 0;

                    // Less than the first texel center, repeat nearest neighbor
                    if (width < 0.5) {
                        w0 = 0;
                        w1 = 0;
                    }
                    // Larger than the last texel center, repeat nearest neighbor
                    else if (width >= static_cast<double>(_width) - 0.5) {
                        w0 = _width - 1;
                        w1 = _width - 1;
                    }
                    // Sampling at texel center
                    else {
                        auto dw = width - std::floor(width);
                        auto wInt = static_cast<std::size_t>(width);
                        if (dw < 0.5) {
                            w0 = wInt - 1;
                            w1 = wInt;
                            ww = dw + 0.5;
                        }
                        else {
                            w0 = wInt;
                            w1 = wInt + 1;
                            ww = dw - 0.5;
                        }
                    }

                    if (height < 0.5) {
                        h0 = 0;
                        h1 = 0;
                    }
                    else if (height >= static_cast<double>(_height) - 0.5) {
                        h0 = _height - 1;
                        h1 = _height - 1;
                    }
                    else {
                        auto dh = height - std::floor(height);
                        auto hInt = static_cast<std::size_t>(height);
                        if (dh < 0.5) {
                            h0 = hInt - 1;
                            h1 = hInt;
                            hw = dh + 0.5;
                        }
                        else {
                            h0 = hInt;
                            h1 = hInt + 1;
                            hw = dh - 0.5;
                        }
                    }

                    auto color0 = at<FORMAT>(w0, h0) * (1 - ww) + at<FORMAT>(w1, h0) * ww;
                    auto color1 = at<FORMAT>(w0, h1) * (1 - ww) + at<FORMAT>(w1, h1) * ww;

                    return color0 * (1 - hw) + color1 * hw;
                }
                default:
                    assert(false);
            }
        }

        template<ImageFormat FORMAT>
        void Image::fill(const Pixel<FORMAT> &filledPixel) {
            assert(static_cast<std::size_t>(FORMAT) <= _format);

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
        Image::Image(std::size_t w, std::size_t h, const Pixel<FORMAT> &filledPixel, FilterType filterType): _format(static_cast<std::size_t>(FORMAT)), _data(h * w * _format), _width(w), _height(h), filterType(filterType) {
            fill<FORMAT>(filledPixel);
        }

        template<ImageFormat FORMAT>
        const Pixel<FORMAT> Image::get(const Math::Vector2D &pos) const {
            return get<FORMAT>(pos.x(), pos.y());
        }

        template<ImageFormat FORMAT>
        const Pixel<FORMAT> Image::at(const Math::Vector2S &pos) const {
            return at<FORMAT>(pos.x(), pos.y());
        }

        template<ImageFormat FORMAT>
        PixelProxy<FORMAT> Image::at(const Math::Vector2S &pos) {
            return at<FORMAT>(pos.x(), pos.y());
        }
    } // UltRenderer
} // Data

#endif //ULTRENDERER_INCLUDE_DATA_IMAGE_H_
