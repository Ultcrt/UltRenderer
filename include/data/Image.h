//
// Created by ultcrt on 23-11-15.
//

#ifndef ULTRENDERER_INCLUDE_DATA_IMAGE_H_
#define ULTRENDERER_INCLUDE_DATA_IMAGE_H_

#include <vector>
#include <array>
#include <string>
#include <fstream>
#include <iostream>
#include <cassert>
#include <cstdint>
#include <exception>
#include <format>
#include <limits>
#include <algorithm>
#include "data/Matrix.h"

namespace UltRenderer {
    namespace Data {
        enum class ImageFormat: std::size_t {
            GRAY = 1, RGB = 3, RGBA = 4
        };

        enum class ImageDirection: std::size_t {
            HORIZONTAL = 1, VERTICAL = 2
        };

        template<ImageFormat FORMAT>
        using Pixel = Matrix<double, static_cast<std::size_t>(FORMAT), 1>;

        template<ImageFormat FORMAT>
        class Image {
        private:
            const std::size_t   _format = static_cast<std::size_t>(FORMAT);
            std::vector<double> _data;
            std::size_t         _width;
            std::size_t         _height;

        public:
            void fill(const Pixel<FORMAT>& filledPixel);
            Image(std::size_t w, std::size_t h);
            Image(std::size_t w, std::size_t h, const Pixel<FORMAT>& filledPixel);
            void set(std::size_t w, std::size_t h, const Pixel<FORMAT>& pixel);
            Pixel<FORMAT> get(std::size_t w, std::size_t h);
            void save(const std::string& filename);
            [[nodiscard]] Vector2S shape() const;
            [[nodiscard]] std::size_t width() const;
            [[nodiscard]] std::size_t height() const;
            void flip(ImageDirection direction);
        };

        template<ImageFormat FORMAT>
        std::size_t Image<FORMAT>::height() const {
            return _height;
        }

        template<ImageFormat FORMAT>
        std::size_t Image<FORMAT>::width() const {
            return _width;
        }

        template<ImageFormat FORMAT>
        void Image<FORMAT>::flip(ImageDirection direction) {
            std::vector<double> newData(_data.size());

            for (std::size_t w = 0; w < _width; w++) {
                for (std::size_t h = 0; h < _height; h++) {
                    for (std::size_t idx = 0; idx < _format; idx++) {
                        if (direction == ImageDirection::HORIZONTAL) {
                            newData[(h * _width + w) * _format + idx] = _data[((_height - h - 1) * _width + w) * _format + idx];
                        }
                        else {
                            newData[(h * _width + w) * _format + idx] = _data[(h * _width + (_width - w - 1)) * _format + idx];
                        }
                    }
                }
            }

            _data = newData;
        }

        template<ImageFormat FORMAT>
        void Image<FORMAT>::fill(const Pixel<FORMAT> &filledPixel) {
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
                std::fill(_data.begin(), _data.end(), filledPixel[0]);
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
        Image<FORMAT>::Image(std::size_t w, std::size_t h, const Pixel<FORMAT> &filledPixel): _data(h * w * _format), _width(w), _height(h) {
            fill(filledPixel);
        }

        template<ImageFormat FORMAT>
        Vector2S Image<FORMAT>::shape() const {
            return {_width, _height};
        }

        template<ImageFormat FORMAT>
        Pixel<FORMAT> Image<FORMAT>::get(std::size_t w, std::size_t h) {
            assert(w < _width);
            assert(h < _height);

            Pixel<FORMAT> pixel;

            for (std::size_t idx = 0; idx < _format; idx++) {
                pixel[idx] = _data[(h * _width + w) * _format + idx];
            }

            return pixel;
        }

        template<ImageFormat FORMAT>
        void Image<FORMAT>::set(std::size_t w, std::size_t h, const Pixel<FORMAT> &pixel) {
            assert(w < _width);
            assert(h < _height);

            for (std::size_t idx = 0; idx < _format; idx++) {
                _data[(h * _width + w) * _format + idx] = pixel[idx];
            }
        }

        template<ImageFormat FORMAT>
        void Image<FORMAT>::save(const std::string &filename) {
            std::ofstream tga(filename, std::ios::binary);
            if (!tga.is_open()) {
                throw std::runtime_error(std::format("Cannot open file: {}", filename));
            }

            // Image ID is optional, ignored here
            std::uint8_t idLen        = 0;
            // Has no color map
            std::uint8_t colorMapType = 0;
            // Gray or not
            std::uint8_t imageType    = FORMAT == ImageFormat::GRAY ? 3 : 2;

            // Has no color map, set related headers to 0
            std::uint16_t firstColorMapEntryIdx = 0;
            std::uint16_t colorMapLen           = 0;
            std::uint8_t  colorMapEntrySize     = 0;


            // Set image related headers
            std::uint16_t xOrigin         = 0;
            std::uint16_t yOrigin         = 0;
            std::uint16_t width           = _width;
            std::uint16_t height          = _height;
            // Bytes number * 8
            std::uint8_t  pixelDepth      = _format << 3;
            // Top-left order
            std::uint8_t  imageDescriptor = 0x20;

            // Write headers
            tga.write(reinterpret_cast<const char *>(&idLen), sizeof(idLen));
            tga.write(reinterpret_cast<const char *>(&colorMapType), sizeof(colorMapType));
            tga.write(reinterpret_cast<const char *>(&imageType), sizeof(imageType));
            tga.write(reinterpret_cast<const char *>(&firstColorMapEntryIdx), sizeof(firstColorMapEntryIdx));
            tga.write(reinterpret_cast<const char *>(&colorMapLen), sizeof(colorMapLen));
            tga.write(reinterpret_cast<const char *>(&colorMapEntrySize), sizeof(colorMapEntrySize));
            tga.write(reinterpret_cast<const char *>(&xOrigin), sizeof(xOrigin));
            tga.write(reinterpret_cast<const char *>(&yOrigin), sizeof(yOrigin));
            tga.write(reinterpret_cast<const char *>(&width), sizeof(width));
            tga.write(reinterpret_cast<const char *>(&height), sizeof(height));
            tga.write(reinterpret_cast<const char *>(&pixelDepth), sizeof(pixelDepth));
            tga.write(reinterpret_cast<const char *>(&imageDescriptor), sizeof(imageDescriptor));

            std::vector<std::uint8_t> reorderedData;
            // Clamp raw data to [0, 1] and rescale to [0, 255]
            for (const auto& unit: _data) {
                reorderedData.emplace_back(std::clamp(unit, 0., 1.) * std::numeric_limits<std::uint8_t>::max());
            }
            // RGB to BGR
            if (FORMAT != ImageFormat::GRAY) {
                // Alpha channel is already filled, only need to reverse RGB
                for (std::size_t pixelIdx = 0; pixelIdx < _data.size() / _format; pixelIdx++) {
                    std::swap(reorderedData[pixelIdx * _format], reorderedData[pixelIdx * _format + 2]);
                }
            }

            // Write data
            tga.write(reinterpret_cast<const char *>(reorderedData.data()), static_cast<long>(reorderedData.size()));
            if (!tga.good()) {
                throw std::runtime_error(std::format("Error occurs when writing to: {}", filename));
            }

            tga.close();
            if (tga.is_open()) {
                throw std::runtime_error(std::format("Cannot close file: {}", filename));
            }
        }

        template<ImageFormat FORMAT>
        Image<FORMAT>::Image(std::size_t w, std::size_t h):
        _data(h * w * _format), _width(w), _height(h) {}

        typedef Image<ImageFormat::GRAY> GrayImage;
        typedef Image<ImageFormat::RGB>  RGBImage;
        typedef Image<ImageFormat::RGBA> RGBAImage;
    } // UltRenderer
} // Data

#endif //ULTRENDERER_INCLUDE_DATA_IMAGE_H_
