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
            void Line(UltRenderer::Data::Image& img, std::size_t x0, std::size_t y0, std::size_t x1, std::size_t y1, const UltRenderer::Data::Pixel<FORMAT>& pixel);

            template<UltRenderer::Data::ImageFormat FORMAT>
            void Line(UltRenderer::Data::Image& img, const Data::Vector2S& p0, const Data::Vector2S& p1, const UltRenderer::Data::Pixel<FORMAT>& pixel);

            template<UltRenderer::Data::ImageFormat FORMAT>
            void Triangle(UltRenderer::Data::Image& img, const std::array<Data::Vector2S, 3> &points,
                          const Data::Vector3D& depths, const std::array<Data::Vector2S, 3> &uvs, const UltRenderer::Data::Image& texture,
                          std::vector<double>& zBuffer);

            template<UltRenderer::Data::ImageFormat FORMAT>
            void Line(UltRenderer::Data::Image& img, std::size_t x0, std::size_t y0, std::size_t x1,
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
                        img.at<FORMAT>(longPos, shortPos) = pixel;
                    }
                    else {
                        img.at<FORMAT>(shortPos, longPos) = pixel;
                    }

                    error += static_cast<long long>(step);
                }
            }

            template<UltRenderer::Data::ImageFormat FORMAT>
            void Line(UltRenderer::Data::Image& img, const Data::Vector2S &p0,
                      const Data::Vector2S &p1, const UltRenderer::Data::Pixel<FORMAT> &pixel) {
                Line<FORMAT>(img, p0.x(), p0.y(), p1.x(), p1.y(), pixel);
            }

            template<UltRenderer::Data::ImageFormat FORMAT>
            void Triangle(UltRenderer::Data::Image& img, const std::array<Data::Vector2S, 3> &points,
                          const Data::Vector3D& depths, const std::array<Data::Vector2S, 3> &uvs, const UltRenderer::Data::Image& texture,
                          std::vector<double>& zBuffer) {
                std::size_t width = img.width();

                std::array<Data::Vector2D, 3> doublePoints = {
                        static_cast<Data::Vector2D>(points[0]),
                        static_cast<Data::Vector2D>(points[1]),
                        static_cast<Data::Vector2D>(points[2])
                };

                std::array<Data::Vector2D, 3> doubleUVs = {
                        static_cast<Data::Vector2D>(uvs[0]),
                        static_cast<Data::Vector2D>(uvs[1]),
                        static_cast<Data::Vector2D>(uvs[2])
                };

                auto [minVec, maxVec] = Utils::Geometry::GetAABB<std::size_t, 2>({points.begin(), points.end()});

                for (std::size_t xIdx = minVec.x(); xIdx < maxVec.x(); xIdx++) {
                    for (std::size_t yIdx = minVec.y(); yIdx < maxVec.y(); yIdx++) {
                        auto barycentricCoords = Utils::Geometry::ComputeBarycentricCoords2D<double>({static_cast<double>(xIdx), static_cast<double>(yIdx)}, doublePoints);

                        if (barycentricCoords.x() >= 0 && barycentricCoords.y() >= 0 && barycentricCoords.z() >= 0) {
                            double depth = barycentricCoords.dot(depths);
                            Data::Vector2S uv = static_cast<Data::Vector2S>(doubleUVs[0] * barycentricCoords[0] + doubleUVs[1] * barycentricCoords[1] + doubleUVs[2] * barycentricCoords[2]);

                            if (depth > zBuffer[yIdx * width + xIdx]) {
                                zBuffer[yIdx * width + xIdx] = depth;
                                img.at<FORMAT>(xIdx, yIdx) = texture.at<FORMAT>(uv.x(), uv.y());
                            }
                        }
                    }
                }
            }
        }
    } // UltRenderer
} // Rendering

#endif //ULTRENDERER_RASTERIZE_H
