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

namespace UltRenderer {
    namespace Data {
        enum ImageFormat {
            GRAY = 1, RGB = 3, RGBA = 4
        };

        template<ImageFormat FORMAT>
        class Image {
        private:
            std::vector<std::uint8_t> _data;
            std::size_t               _width;
            std::size_t               _height;

        public:
            Image(std::size_t w, std::size_t h);
            void set(std::size_t w, std::size_t h, const std::array<std::size_t, FORMAT>& color);
            std::array<std::size_t, FORMAT> get(std::size_t w, std::size_t h);
            bool save(const std::string& filename);
        };

        template<ImageFormat FORMAT>
        std::array<std::size_t, FORMAT> Image<FORMAT>::get(std::size_t w, std::size_t h) {
            std::array<std::size_t, FORMAT> color;

            for (std::size_t idx = 0; idx < FORMAT; idx++) {
                color[idx] = _data[(h * _width + w) * FORMAT + idx];
            }
        }

        template<ImageFormat FORMAT>
        void Image<FORMAT>::set(std::size_t w, std::size_t h, const std::array<std::size_t, FORMAT> &color) {
            for (std::size_t idx = 0; idx < FORMAT; idx++) {
                _data[(h * _width + w) * FORMAT + idx] = color[idx];
            }
        }

        template<ImageFormat FORMAT>
        bool Image<FORMAT>::save(const std::string &filename) {
            std::ofstream tga(filename, std::ios::binary);
            if (!tga.is_open()) {
                std::cerr << "Cannot open file: " << filename << std::endl;
                return false;
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
            std::vector<std::uint8_t> reorderedData = _data;
            if (FORMAT != GRAY) {
                // Alpha channel remains the same
                for (std::size_t pixelIdx = 0; pixelIdx < _data.size() / FORMAT; pixelIdx++) {
                    for (std::size_t channelIdx = 0; channelIdx < 3; channelIdx++) {
                        reorderedData[pixelIdx * FORMAT + channelIdx] = _data[pixelIdx * FORMAT + 2 - channelIdx];
                    }
                }
            }

            // Write data
            tga.write(reinterpret_cast<const char *>(reorderedData.data()), reorderedData.size());
            if (!tga.good()) {
                std::cerr << "Error occurs when writing to: " << filename << std::endl;
                return false;
            }

            tga.close();
            if (tga.is_open()) {
                std::cerr << "Cannot close file: " << filename << std::endl;
                return false;
            }

            return true;
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
