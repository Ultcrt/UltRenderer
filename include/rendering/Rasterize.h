//
// Created by ultcrt on 23-11-21.
//

#ifndef ULTRENDERER_RASTERIZE_H
#define ULTRENDERER_RASTERIZE_H

#include <cmath>
#include "data/Matrix.h"
#include "data/Image.h"
#include "utils/Geometry.h"

namespace UltRenderer {
    namespace Rendering {
        namespace Rasterize {
            template<UltRenderer::Data::ImageFormat FORMAT>
            void Line(UltRenderer::Data::Image<FORMAT>& img, std::size_t x0, std::size_t y0, std::size_t x1, std::size_t y1, const UltRenderer::Data::Pixel<FORMAT>& pixel);

            template<UltRenderer::Data::ImageFormat FORMAT>
            void Line(UltRenderer::Data::Image<FORMAT>& img, const Data::Vector2S& p0, const Data::Vector2S& p1, const UltRenderer::Data::Pixel<FORMAT>& pixel);

            template<UltRenderer::Data::ImageFormat FORMAT>
            void Triangle(UltRenderer::Data::Image<FORMAT> &img, const std::array<Data::Vector2S, 3> &points,
                          const Data::Vector3D& depths, const UltRenderer::Data::Pixel<FORMAT> &pixel,
                          std::vector<double>& zBuffer);

            template<UltRenderer::Data::ImageFormat FORMAT>
            void Line(UltRenderer::Data::Image<FORMAT> &img, std::size_t x0, std::size_t y0, std::size_t x1,
                      std::size_t y1, const UltRenderer::Data::Pixel<FORMAT> &pixel) {
                const auto dX = static_cast<std::size_t>(std::abs(static_cast<double>(x0) - static_cast<double>(x1)));
                const auto dY = static_cast<std::size_t>(std::abs(static_cast<double>(y0) - static_cast<double>(y1)));

                // Set longer axis as sampling direction
                bool xIsLonger = dX >= dY;

                std::size_t samples, longOrigin, shortOrigin;
                std::size_t step;
                bool shortIsIncreasing;
                if (xIsLonger) {
                    samples = dX;
                    if (x0 <= x1) {
                        longOrigin = x0;
                        shortOrigin = y0;
                        shortIsIncreasing = y0 < y1;
                    }
                    else {
                        longOrigin = x1;
                        shortOrigin = y1;
                        shortIsIncreasing = y1 < y0;
                    }
                    step = dY;
                }
                else {
                    samples = dY;
                    if (y0 <= y1) {
                        longOrigin = y0;
                        shortOrigin = x0;
                        shortIsIncreasing = x0 < x1;
                    }
                    else {
                        longOrigin = y1;
                        shortOrigin = x1;
                        shortIsIncreasing = x1 < x0;
                    }
                    step = dX;
                }

                long long error = 0;
                std::size_t shortPos = shortOrigin;
                for (std::size_t sampleIdx = 0; sampleIdx < samples; sampleIdx++) {
                    const std::size_t longPos = longOrigin + sampleIdx;

                    if (2 * error > static_cast<long long>(samples)) {
                        shortPos += shortIsIncreasing ? 1 : -1;
                        error -= static_cast<long long>(samples);
                    }

                    if (xIsLonger) {
                        img(longPos, shortPos) = pixel;
                    }
                    else {
                        img(shortPos, longPos) = pixel;
                    }

                    error += static_cast<long long>(step);
                }
            }

            template<UltRenderer::Data::ImageFormat FORMAT>
            void Line(UltRenderer::Data::Image<FORMAT> &img, const Data::Vector2S &p0,
                      const Data::Vector2S &p1, const UltRenderer::Data::Pixel<FORMAT> &pixel) {
                Line<FORMAT>(img, p0.x(), p0.y(), p1.x(), p1.y(), pixel);
            }

            template<UltRenderer::Data::ImageFormat FORMAT>
            void Triangle(UltRenderer::Data::Image<FORMAT> &img, const std::array<Data::Vector2S, 3> &points,
                          const Data::Vector3D& depths, const UltRenderer::Data::Pixel<FORMAT> &pixel,
                          std::vector<double>& zBuffer) {
                std::size_t width = img.width();

                std::array<Data::Vector2D, 3> doublePoints = {
                        static_cast<Data::Vector2D>(points[0]),
                        static_cast<Data::Vector2D>(points[1]),
                        static_cast<Data::Vector2D>(points[2])
                };                auto [minVec, maxVec] = Utils::Geometry::GetAABB<std::size_t, 2>({points.begin(), points.end()});

                for (std::size_t xIdx = minVec.x(); xIdx < maxVec.x(); xIdx++) {
                    for (std::size_t yIdx = minVec.y(); yIdx < maxVec.y(); yIdx++) {
                        auto barycentricCoords = Utils::Geometry::ComputeBarycentricCoords2D<double>({static_cast<double>(xIdx), static_cast<double>(yIdx)}, doublePoints);

                        if (barycentricCoords.x() >= 0 && barycentricCoords.y() >= 0 && barycentricCoords.z() >= 0) {
                            double depth = barycentricCoords.dot(depths);

                            if (depth > zBuffer[yIdx * width + xIdx]) {
                                zBuffer[yIdx * width + xIdx] = depth;
                                img(xIdx, yIdx) = pixel;
                            }
                        }
                    }
                }
            }
        }
    } // UltRenderer
} // Rendering

#endif //ULTRENDERER_RASTERIZE_H
