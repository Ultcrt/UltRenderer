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
#include "data/Matrix.h"

namespace UltRenderer {
    namespace Data {
        enum ImageFormat {
            GRAY = 1, RGB = 3, RGBA = 4
        };

        template<ImageFormat FORMAT>
        using Pixel = Matrix<double, FORMAT, 1>;

        template<ImageFormat FORMAT>
        class Image {
        private:
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
        };

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
                        for (std::size_t idx = 0; idx < FORMAT; idx++) {
                            _data[(h * _width + w) * FORMAT + idx] = filledPixel[idx];
                        }
                    }
                }
            }
        }

        template<ImageFormat FORMAT>
        Image<FORMAT>::Image(std::size_t w, std::size_t h, const Pixel<FORMAT> &filledPixel): _data(h * w * FORMAT), _width(w), _height(h) {
            fill(filledPixel);
        }

        template<ImageFormat FORMAT>
        Vector2S Image<FORMAT>::shape() const {
            return {_height, _width};
        }

        template<ImageFormat FORMAT>
        Pixel<FORMAT> Image<FORMAT>::get(std::size_t w, std::size_t h) {
            assert(w < _width);
            assert(h < _height);

            Pixel<FORMAT> pixel;

            for (std::size_t idx = 0; idx < FORMAT; idx++) {
                pixel[idx] = _data[(h * _width + w) * FORMAT + idx];
            }

            return pixel;
        }

        template<ImageFormat FORMAT>
        void Image<FORMAT>::set(std::size_t w, std::size_t h, const Pixel<FORMAT> &pixel) {
            assert(w < _width);
            assert(h < _height);

            for (std::size_t idx = 0; idx < FORMAT; idx++) {
                _data[(h * _width + w) * FORMAT + idx] = pixel[idx];
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
            std::uint8_t imageType    = FORMAT == GRAY ? 3 : 2;

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
            std::uint8_t  pixelDepth      = FORMAT << 3;
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

            // RGB to BGR
            std::vector<std::uint8_t> reorderedData;
            for (const auto& unit: _data) {
                reorderedData.emplace_back(std::min(1.0, unit) * std::numeric_limits<std::uint8_t>::max());
            }
            if (FORMAT != GRAY) {
                // Alpha channel remains the same
                for (std::size_t pixelIdx = 0; pixelIdx < _data.size() / FORMAT; pixelIdx++) {
                    for (std::size_t channelIdx = 0; channelIdx < 3; channelIdx++) {
                        reorderedData[pixelIdx * FORMAT + channelIdx] = _data[pixelIdx * FORMAT + 2 - channelIdx];
                    }
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
        _data(h * w * FORMAT), _width(w), _height(h) {}

        typedef Image<GRAY> GrayImage;
        typedef Image<RGB>  RGBImage;
        typedef Image<RGBA> RGBAImage;
    } // UltRenderer
} // Data

#endif //ULTRENDERER_INCLUDE_DATA_IMAGE_H_
