//
// Created by ultcrt on 23-11-21.
//

#ifndef ULTRENDERER_RASTERIZE_H
#define ULTRENDERER_RASTERIZE_H

#include <cmath>
#include <iostream>
#include "math/Matrix.h"
#include "data/Image.h"
#include "math/Geometry.h"

namespace UltRenderer {
    namespace Rendering {
        namespace Rasterize {
            /*----------Declaration----------*/
            template<UltRenderer::Data::ImageFormat FORMAT>
            void Line(UltRenderer::Data::Image& img, const Math::Vector2S& p0, const Math::Vector2S& p1, const UltRenderer::Data::Pixel<FORMAT>& pixel);

            template<UltRenderer::Data::ImageFormat FORMAT>
            void Triangle(UltRenderer::Data::Image& img, const std::array<Math::Vector2S, 3> &points,
                          const Math::Vector3D& depths, const std::array<Math::Vector3D, 3> &uvs, const UltRenderer::Data::Image& texture, double colorScale,
                          UltRenderer::Data::Image& zBuffer);

            /*----------Definition----------*/
            template<UltRenderer::Data::ImageFormat FORMAT>
            void Line(UltRenderer::Data::Image& img, const Math::Vector2S &p0,
                      const Math::Vector2S &p1, const UltRenderer::Data::Pixel<FORMAT> &pixel) {
                const auto dX = static_cast<std::size_t>(std::abs(static_cast<double>(p0.x()) - static_cast<double>(p1.x())));
                const auto dY = static_cast<std::size_t>(std::abs(static_cast<double>(p0.y()) - static_cast<double>(p1.y())));

                // Set longer axis as sampling direction
                bool xIsLonger = dX >= dY;

                std::size_t samples, longOrigin, shortOrigin;
                std::size_t step;
                bool shortIsIncreasing;
                if (xIsLonger) {
                    samples = dX;
                    if (p0.x() <= p1.x()) {
                        longOrigin = p0.x();
                        shortOrigin = p0.y();
                        shortIsIncreasing = p0.y() < p1.y();
                    }
                    else {
                        longOrigin = p1.x();
                        shortOrigin = p1.y();
                        shortIsIncreasing = p1.y() < p0.y();
                    }
                    step = dY;
                }
                else {
                    samples = dY;
                    if (p0.y() <= p1.y()) {
                        longOrigin = p0.y();
                        shortOrigin = p0.x();
                        shortIsIncreasing = p0.x() < p1.x();
                    }
                    else {
                        longOrigin = p1.y();
                        shortOrigin = p1.x();
                        shortIsIncreasing = p1.x() < p0.x();
                    }
                    step = dX;
                }

                long long error = 0;
                std::size_t shortPos = shortOrigin;
                for (std::size_t sampleIdx = 0; sampleIdx < samples; sampleIdx++) {
                    const std::size_t longPos = longOrigin + sampleIdx;

                    // error multiply 2 means the center point of the pixel represent the whole pixel
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
            void Triangle(UltRenderer::Data::Image& img, const std::array<Math::Vector2S, 3> &points,
                          const Math::Vector3D& depths, const std::array<Math::Vector3D, 3> &uvs, const UltRenderer::Data::Image& texture, double colorScale,
                          UltRenderer::Data::Image& zBuffer) {
                Math::Vector3D textureShapeVec(static_cast<double >(texture.width()), static_cast<double >(texture.height()), 0);

                std::array<Math::Vector2D, 3> doublePoints = {
                        static_cast<Math::Vector2D>(points[0]),
                        static_cast<Math::Vector2D>(points[1]),
                        static_cast<Math::Vector2D>(points[2])
                };

                std::array<Math::Vector3D, 3> scaledUVs = {
                        uvs[0].componentWiseProduct(textureShapeVec),
                        uvs[1].componentWiseProduct(textureShapeVec),
                        uvs[2].componentWiseProduct(textureShapeVec)
                };

                auto [minVec, maxVec] = Utils::Geometry::GetAABB<std::size_t, 2>({points.begin(), points.end()});

                for (std::size_t xIdx = minVec.x(); xIdx <= maxVec.x(); xIdx++) {
                    for (std::size_t yIdx = minVec.y(); yIdx <= maxVec.y(); yIdx++) {
                        // TODO: Barycentric coordinates is not the same before and after perspective projection
                        auto barycentricCoords = Utils::Geometry::ComputeBarycentricCoords2D({static_cast<double>(xIdx) + 0.5, static_cast<double>(yIdx) + 0.5}, doublePoints);

                        if (barycentricCoords.x() >= 0 && barycentricCoords.y() >= 0 && barycentricCoords.z() >= 0) {
                            double depth = barycentricCoords.dot(depths);
                            Math::Vector3S uv = static_cast<Math::Vector3S>(scaledUVs[0] * barycentricCoords[0] + scaledUVs[1] * barycentricCoords[1] + scaledUVs[2] * barycentricCoords[2]);

                            if (depth > zBuffer.at<Data::ImageFormat::GRAY>(xIdx, yIdx)[0]) {
                                zBuffer.at<Data::ImageFormat::GRAY>(xIdx, yIdx)[0] = depth;
                                // TODO: Not support 3D texture for now
                                img.at<FORMAT>(xIdx, yIdx) = static_cast<Data::Pixel<FORMAT>>(texture.at<FORMAT>(uv.x(), uv.y())) * colorScale;
                            }
                        }
                    }
                }
            }
        }
    } // UltRenderer
} // Rendering

#endif //ULTRENDERER_RASTERIZE_H
