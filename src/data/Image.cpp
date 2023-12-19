//
// Created by ultcrt on 23-11-15.
//

#include <bitset>
#include <string>
#include <fstream>
#include <iostream>
#include <exception>
#include <format>
#include <limits>
#include <algorithm>
#include <cstdint>
#include "../../include/data/Image.h"

namespace UltRenderer {
    namespace Data {
        Image::Image(std::size_t w, std::size_t h, ImageFormat format):
                _format(static_cast<std::size_t>(format)), _width(w), _height(h), _data(h * w * _format) {}

        Vector2S Image::shape() const {
            return {_width, _height};
        }

        std::size_t Image::height() const {
            return _height;
        }

        std::size_t Image::width() const {
            return _width;
        }

        void Image::flip(ImageDirection direction) {
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

        void Image::save(const std::string &filename) {
            std::ofstream tga(filename, std::ios::binary);
            if (!tga.is_open()) {
                throw std::runtime_error(std::format("Cannot open file: {}", filename));
            }

            // Image ID is optional, ignored here
            std::uint8_t idLen        = 0;
            // Has no color map
            std::uint8_t colorMapType = 0;
            // Gray or not
            std::uint8_t imageType    = static_cast<ImageFormat>(_format) == ImageFormat::GRAY ? 3 : 2;

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
            std::uint8_t  imageDescriptor = static_cast<ImageFormat>(_format) == ImageFormat::RGBA ? 0b00001000 : 0b00000000;

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
            if (static_cast<ImageFormat>(_format) != ImageFormat::GRAY) {
                // Alpha channel is already filled, only need to reverse RGB
                for (std::size_t pixelIdx = 0; pixelIdx < width * height; pixelIdx++) {
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

        Image::Image(const std::string &filename) {
            std::ifstream tga(filename, std::ios::binary);
            if (!tga.is_open()) {
                throw std::runtime_error(std::format("Cannot open file: {}", filename));
            }

            // Image ID
            std::uint8_t idLen;
            // Color map type
            std::uint8_t colorMapType;
            // Image type
            std::uint8_t imageType;

            // Color map
            std::uint16_t firstColorMapEntryIdx;
            std::uint16_t colorMapLen;
            std::uint8_t  colorMapEntrySize;

            // Image related headers
            std::uint16_t xOrigin;
            std::uint16_t yOrigin;
            std::uint16_t width;
            std::uint16_t height;
            // Bytes number * 8
            std::uint8_t  pixelDepth;
            // Pixel order
            std::uint8_t  imageDescriptor;

            // Read headers
            tga.read(reinterpret_cast<char *>(&idLen), sizeof(idLen));
            tga.read(reinterpret_cast<char *>(&colorMapType), sizeof(colorMapType));
            tga.read(reinterpret_cast<char *>(&imageType), sizeof(imageType));
            tga.read(reinterpret_cast<char *>(&firstColorMapEntryIdx), sizeof(firstColorMapEntryIdx));
            tga.read(reinterpret_cast<char *>(&colorMapLen), sizeof(colorMapLen));
            tga.read(reinterpret_cast<char *>(&colorMapEntrySize), sizeof(colorMapEntrySize));
            tga.read(reinterpret_cast<char *>(&xOrigin), sizeof(xOrigin));
            tga.read(reinterpret_cast<char *>(&yOrigin), sizeof(yOrigin));
            tga.read(reinterpret_cast<char *>(&width), sizeof(width));
            tga.read(reinterpret_cast<char *>(&height), sizeof(height));
            tga.read(reinterpret_cast<char *>(&pixelDepth), sizeof(pixelDepth));
            tga.read(reinterpret_cast<char *>(&imageDescriptor), sizeof(imageDescriptor));

            // TODO: Only support 8 bits color channel
            _width = width;
            _height = height;
            _format = pixelDepth >> 3;
            _data = std::vector<double>(_width * _height * _format);

            // TODO: Only resolve data field, skip image ID field and color map field
            tga.seekg(idLen, std::ios::cur);
            tga.seekg(colorMapLen * colorMapEntrySize / 8, std::ios::cur);

            std::vector<std::uint8_t> imageData;

            // TODO: Cannot process color map for now
            // Run-length data process
            if (imageType == 9 || imageType == 10 || imageType == 11) {
                // Obtain raw bytes
                std::uint8_t header;
                while (tga.read(reinterpret_cast<char *>(&header), sizeof(header))) {
                    std::uint8_t id    = header >> 7;
                    // Count is defined as 7 bit + 1
                    std::uint8_t count = (header & 0b01111111) + 1;

                    // Run-length packet
                    if (id == 1) {
                        std::vector<std::uint8_t> runLengthColor(_format);
                        tga.read(reinterpret_cast<char *>(runLengthColor.data()), static_cast<long>(runLengthColor.size()));

                        for (std::size_t idx = 0; idx < count; idx++) {
                            imageData.insert(imageData.end(), runLengthColor.begin(), runLengthColor.end());
                        }
                    }
                    // Raw packet
                    else {
                        std::vector<std::uint8_t> rawColors(count * _format);
                        tga.read(reinterpret_cast<char *>(rawColors.data()), static_cast<long>(rawColors.size()));
                        if (tga.good()) {

                        }
                        imageData.insert(imageData.end(), rawColors.begin(), rawColors.end());
                    }
                }
            }
            // Uncompressed data process
            else {
                tga.read(reinterpret_cast<char *>(imageData.data()), static_cast<long>(width * height * _format));
            }

            for (std::size_t idx = _data.size(); idx < imageData.size(); idx++) {
                std::cout << std::bitset<8>(imageData[idx]) << std::endl;
            }

            // Clean up
            if (!tga.good()) {
                throw std::runtime_error(std::format("Error occurs when reading: {}", filename));
            }
            tga.close();
            if (tga.is_open()) {
                throw std::runtime_error(std::format("Cannot close file: {}", filename));
            }

            if (imageData.size() != _data.size()) {
                throw std::runtime_error(std::format("TGA file has unexpected size of image data field: {}", filename));
            }

            // BGR to RGB
            if (static_cast<ImageFormat>(_format) != ImageFormat::GRAY) {
                // Alpha channel is already filled, only need to reverse RGB
                for (std::size_t pixelIdx = 0; pixelIdx < width * height; pixelIdx++) {
                    std::swap(imageData[pixelIdx * _format], imageData[pixelIdx * _format + 2]);
                }
            }

            // Rescale to 0.0~1.0
            for (std::size_t idx = 0; idx < _data.size(); idx++) {
                _data[idx] = (static_cast<double>(imageData[idx]) / static_cast<double>(std::numeric_limits<std::uint8_t>::max()));
            }
        }

        ImageFormat Image::type() const {
            return static_cast<ImageFormat>(_format);
        }
    } // UltRenderer
} // Data