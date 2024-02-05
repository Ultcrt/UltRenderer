//
// Created by ultcrt on 23-11-21.
//

#ifndef ULTRENDERER_RASTERIZING_H
#define ULTRENDERER_RASTERIZING_H

#include <cmath>
#include <iostream>
#include "math/Matrix.h"
#include "data/Image.h"
#include "math/Geometry.h"
#include "rendering/rasterizing/shaders/IShader.h"

namespace UltRenderer {
    namespace Rendering {
        namespace Rasterizing {
            /*----------Declaration----------*/
            template<std::derived_from<Shaders::IVarying> V>
            void Line(UltRenderer::Data::Image& fBuffer, UltRenderer::Data::Image& zBuffer, const std::array<V, 2>& varyings,
                      const Shaders::IFragmentShader<V>& fragmentShader, const Shaders::IInterpolator<V>& interpolator = {});

            template<std::derived_from<Shaders::IVarying> V>
            void Triangle(UltRenderer::Data::Image& fBuffer, UltRenderer::Data::Image& zBuffer, const std::array<Math::Vector4D, 3>& preciseFragCoords, const std::array<V, 3>& varyings,
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

                    // TODO: Need to update pZBuffer
                    double depth = 0;
                    if (xIsLonger) {
                        const Math::Vector2D doubleP = {static_cast<double>(longPos), static_cast<double>(shortPos)};
                        const Math::Vector2D weights = {(doubleP - doubleP0).norm() / len, (doubleP - doubleP1).norm() / len};
                        const V& interpolatedVarying = interpolator(varyings, weights);

                        Math::Vector4D color = fBuffer.at<Data::ColorFormat::RGBA>(longPos, shortPos);
                        // TODO: Need to deal with discard
                        fragmentShader(interpolatedVarying, color, depth);
                        fBuffer.at<Data::ColorFormat::RGBA>(longPos, shortPos) = color;
                    }
                    else {
                        const Math::Vector2D doubleP = {static_cast<double>(shortPos), static_cast<double>(longPos)};
                        const Math::Vector2D weights = {(doubleP - doubleP0).norm() / len, (doubleP - doubleP1).norm() / len};
                        const V& interpolatedVarying = interpolator(varyings, weights);

                        Math::Vector4D color = fBuffer.at<Data::ColorFormat::RGBA>(shortPos, longPos);
                        fragmentShader(interpolatedVarying, color, depth);
                        fBuffer.at<Data::ColorFormat::RGBA>(shortPos, longPos) = color;
                    }

                    error += static_cast<long long>(step);
                }
            }

            template<std::derived_from<Shaders::IVarying> V>
            void Triangle(UltRenderer::Data::Image& fBuffer, UltRenderer::Data::Image& zBuffer, const std::array<Math::Vector4D, 3>& preciseFragCoords, const std::array<V, 3>& varyings,
                          const Shaders::IFragmentShader<V>& fragmentShader, const Shaders::IInterpolator<V>& interpolator) {
                std::array<Math::Vector2D, 3> points = {
                    Math::Vector2D{preciseFragCoords[0].x(), preciseFragCoords[0].y()},
                    Math::Vector2D{preciseFragCoords[1].x(), preciseFragCoords[1].y()},
                    Math::Vector2D{preciseFragCoords[2].x(), preciseFragCoords[2].y()},
                };

                auto [preciseMin, preciseMax] = Math::Geometry::GetMinMax<double, 2>({points.begin(), points.end()});

                // Extend min-max to make sure covering all pixel intersect with bounding box
                Math::Vector2S minVec = {
                        static_cast<std::size_t>(std::floor(preciseMin.x())),
                        static_cast<std::size_t>(std::floor(preciseMin.y()))
                };
                Math::Vector2S maxVec = {
                        static_cast<std::size_t>(std::ceil(preciseMax.x())),
                        static_cast<std::size_t>(std::ceil(preciseMax.y()))
                };

                for (std::size_t xIdx = minVec.x(); xIdx <= maxVec.x(); xIdx++) {
                    for (std::size_t yIdx = minVec.y(); yIdx <= maxVec.y(); yIdx++) {
                        // Get barycentric coordinate in screen space
                        auto fragBarycentricCoord = Math::Geometry::ComputeBarycentricCoords({static_cast<double>(xIdx) + 0.5, static_cast<double>(yIdx) + 0.5}, points);

                        // Check the point is inside triangle or not
                        if (fragBarycentricCoord.x() >= 0 && fragBarycentricCoord.y() >= 0 && fragBarycentricCoord.z() >= 0) {
                            // Perform perspective correction
                            auto clipBarycentricCoord = Math::Vector3D{
                                    fragBarycentricCoord.x() * preciseFragCoords[0].w(),
                                    fragBarycentricCoord.y() * preciseFragCoords[1].w(),
                                    fragBarycentricCoord.z() * preciseFragCoords[2].w()
                            } / (fragBarycentricCoord.x() * preciseFragCoords[0].w() + fragBarycentricCoord.y() * preciseFragCoords[1].w() + fragBarycentricCoord.z() * preciseFragCoords[2].w());

                            V interpolatedVarying = interpolator(varyings, clipBarycentricCoord);

                            Math::Vector4D color = fBuffer.at<Data::ColorFormat::RGBA>(xIdx, yIdx);

                            // TODO: gl_FragDepth not work correctly, depth test should be done before or after fragment shader depending on gl_FragDepth is set or not
                            double depth = preciseFragCoords[0].z() * clipBarycentricCoord[0] + preciseFragCoords[1].z() * clipBarycentricCoord[1] + preciseFragCoords[2].z() * clipBarycentricCoord[2];
                            const double w = 1 / preciseFragCoords[0].w() * clipBarycentricCoord[0] + 1 / preciseFragCoords[1].w() * clipBarycentricCoord[1] + 1 / preciseFragCoords[2].w() * clipBarycentricCoord[2];

                            // Tips: Should not calculate xy using barycentric coord, because float error would make result not the same as xIdx + 0.5, yIdx + 0.5
                            const Math::Vector4D fragCoord = {static_cast<double>(xIdx) + 0.5, static_cast<double>(yIdx) + 0.5, depth, 1 / w};

                            // Only update when not discard
                            if (fragmentShader(interpolatedVarying, fragCoord, color, depth)) {
                                if (depth < zBuffer.at<Data::ColorFormat::GRAY>(xIdx, yIdx)[0]) {
                                    zBuffer.at<Data::ColorFormat::GRAY>(xIdx, yIdx)[0] = depth;
                                    fBuffer.at<Data::ColorFormat::RGBA>(xIdx, yIdx) = color;
                                }
                            }
                        }
                    }
                }
            }
        }
    } // UltRenderer
} // Rendering

#endif //ULTRENDERER_RASTERIZING_H
