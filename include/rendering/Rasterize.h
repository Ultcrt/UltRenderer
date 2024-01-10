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
#include "shaders/IShader.h"

namespace UltRenderer {
    namespace Rendering {
        namespace Rasterize {
            /*----------Declaration----------*/
            template<std::derived_from<Shaders::IVarying> V>
            void Line(UltRenderer::Data::Image& fBuffer, UltRenderer::Data::Image& zBuffer, const std::array<V, 2>& varyings,
                      const Shaders::IFragmentShader<V>& fragmentShader, const Shaders::IInterpolator<V>& interpolator = {});

            template<std::derived_from<Shaders::IVarying> V>
            void Triangle(UltRenderer::Data::Image& fBuffer, UltRenderer::Data::Image& zBuffer, const std::array<V, 3>& varyings,
                          const Shaders::IFragmentShader<V>& fragmentShader, const Shaders::IInterpolator<V>& interpolator = {});

            /*----------Definition----------*/
            template<std::derived_from<Shaders::IVarying> V>
            void Line(UltRenderer::Data::Image& fBuffer, UltRenderer::Data::Image& zBuffer, const std::array<V, 2>& varyings,
                      const Shaders::IFragmentShader<V>& fragmentShader, const Shaders::IInterpolator<V>& interpolator) {
                const Math::Vector2S p0 = {varyings[0].position.x(), varyings[0].position.y()};
                const Math::Vector2S p1 = {varyings[1].position.x(), varyings[1].position.y()};

                const Math::Vector2D doubleP0 = {varyings[0].position.x(), varyings[0].position.y()};
                const Math::Vector2D doubleP1 = {varyings[1].position.x(), varyings[1].position.y()};
                const double len = (doubleP0 - doubleP1).norm();

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

                    // TODO: Need to update zBuffer
                    double depth = 0;
                    if (xIsLonger) {
                        const Math::Vector2D doubleP = {static_cast<double>(longPos), static_cast<double>(shortPos)};
                        const Math::Vector2D weights = {(doubleP - doubleP0).norm() / len, (doubleP - doubleP1).norm() / len};
                        const V& interpolatedVarying = interpolator(varyings, weights);

                        Math::Vector4D color = fBuffer.at<Data::ImageFormat::RGBA>(longPos, shortPos);
                        // TODO: Need to deal with discard
                        fragmentShader(interpolatedVarying, color, depth);
                        fBuffer.at<Data::ImageFormat::RGBA>(longPos, shortPos) = color;
                    }
                    else {
                        const Math::Vector2D doubleP = {static_cast<double>(shortPos), static_cast<double>(longPos)};
                        const Math::Vector2D weights = {(doubleP - doubleP0).norm() / len, (doubleP - doubleP1).norm() / len};
                        const V& interpolatedVarying = interpolator(varyings, weights);

                        Math::Vector4D color = fBuffer.at<Data::ImageFormat::RGBA>(shortPos, longPos);
                        fragmentShader(interpolatedVarying, color, depth);
                        fBuffer.at<Data::ImageFormat::RGBA>(shortPos, longPos) = color;
                    }

                    error += static_cast<long long>(step);
                }
            }

            template<std::derived_from<Shaders::IVarying> V>
            void Triangle(UltRenderer::Data::Image& fBuffer, UltRenderer::Data::Image& zBuffer, const std::array<V, 3>& varyings,
                          const Shaders::IFragmentShader<V>& fragmentShader, const Shaders::IInterpolator<V>& interpolator) {
                std::array<Math::Vector2S, 3> points = {
                        Math::Vector2S{static_cast<std::size_t>(varyings[0].position.x()), static_cast<std::size_t>(varyings[0].position.y())},
                        Math::Vector2S{static_cast<std::size_t>(varyings[1].position.x()), static_cast<std::size_t>(varyings[1].position.y())},
                        Math::Vector2S{static_cast<std::size_t>(varyings[2].position.x()), static_cast<std::size_t>(varyings[2].position.y())},
                };

                std::array<Math::Vector2D, 3> doublePoints = {
                        Math::Vector2D{varyings[0].position.x(), varyings[0].position.y()},
                        Math::Vector2D{varyings[1].position.x(), varyings[1].position.y()},
                        Math::Vector2D{varyings[2].position.x(), varyings[2].position.y()},
                };

                auto [minVec, maxVec] = Utils::Geometry::GetAABB<std::size_t, 2>({points.begin(), points.end()});

                for (std::size_t xIdx = minVec.x(); xIdx <= maxVec.x(); xIdx++) {
                    for (std::size_t yIdx = minVec.y(); yIdx <= maxVec.y(); yIdx++) {
                        auto barycentricCoords = Utils::Geometry::ComputeBarycentricCoords2D({static_cast<double>(xIdx) + 0.5, static_cast<double>(yIdx) + 0.5}, doublePoints);

                        // Check the point is inside triangle or not
                        if (barycentricCoords.x() >= 0 && barycentricCoords.y() >= 0 && barycentricCoords.z() >= 0) {
                            const V& interpolatedVarying = interpolator(varyings, barycentricCoords);
                            Math::Vector4D color = fBuffer.at<Data::ImageFormat::RGBA>(xIdx, yIdx);
                            // TODO: Barycentric coordinates is not the same before and after perspective projection (perspective correct)
                            double depth = interpolatedVarying.position.z();

                            // Only update when not discard
                            if (fragmentShader(interpolatedVarying, color, depth)) {
                                if (depth > zBuffer.at<Data::ImageFormat::GRAY>(xIdx, yIdx)[0]) {
                                    zBuffer.at<Data::ImageFormat::GRAY>(xIdx, yIdx)[0] = depth;
                                    fBuffer.at<Data::ImageFormat::RGBA>(xIdx, yIdx) = color;
                                }
                            }
                        }
                    }
                }
            }
        }
    } // UltRenderer
} // Rendering

#endif //ULTRENDERER_RASTERIZE_H
