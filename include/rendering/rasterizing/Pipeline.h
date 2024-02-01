//
// Created by ultcrt on 24-1-6.
//

#ifndef ULTRENDERER_PIPELINE_H
#define ULTRENDERER_PIPELINE_H

#include <vector>
#include <concepts>
#include <thread>
#include "math/Matrix.h"
#include "data/Image.h"
#include "shaders/IShader.h"
#include "Rasterizing.h"
#include "math/Transform.h"
#include "postprocessors/EmptyPostprocessor.h"

namespace UltRenderer {
    namespace Rendering {
        namespace Rasterizing {
            /*----------Declaration----------*/
            class Pipeline {
            public:
                template <std::derived_from<Shaders::IVarying> V>
                static void Execute(Data::Image& fBuffer, Data::Image& zBuffer, const Math::Transform3D& viewport, std::size_t vertexNum,
                                    const std::vector<Math::Vector3S>& triangles, const std::vector<Math::Vector2S>& lines, const std::vector<std::size_t>& points,
                                    const Shaders::IVertexShader<V>& vertexShader,
                                    const Shaders::IFragmentShader<V>& fragmentShader,
                                    const Shaders::IInterpolator<V>& interpolator,
                                    const Postprocessors::IPostprocessor& postprocessor = Postprocessors::EmptyPostprocessor(),
                                    std::size_t maxThreads = 20);
            };

            /*----------Definition----------*/
            // TODO: Only support triangle primitives for now
            template <std::derived_from<Shaders::IVarying> V>
            void Pipeline::Execute(Data::Image& fBuffer, Data::Image& zBuffer, const Math::Transform3D& viewport, std::size_t vertexNum,
                                   const std::vector<Math::Vector3S>& triangles, const std::vector<Math::Vector2S>& lines, const std::vector<std::size_t>& points,
                                   const Shaders::IVertexShader<V>& vertexShader,
                                   const Shaders::IFragmentShader<V>& fragmentShader,
                                   const Shaders::IInterpolator<V>& interpolator,
                                   const Postprocessors::IPostprocessor& postprocessor, std::size_t maxThreads) {
                // Process vertex
                std::vector<V> varyings(vertexNum);
                std::vector<Math::Vector4D> preciseFragCoords(vertexNum);
                std::vector<bool> clipFlags(vertexNum);

                std::vector<std::thread> threads;
                for (std::size_t vIdx = 0; vIdx < vertexNum; vIdx++) {
                    threads.emplace_back([&vertexShader, &varyings, &preciseFragCoords, &clipFlags, &viewport](std::size_t vIdx){
                        // Call vertex shader for each vertex
                        Math::Vector4D position;
                        V varying = vertexShader(vIdx, position);

                        // TODO: Clipping is not ideal (discard triangle when one vertex is outside)
                        // Record vertex clip flag here
                        const double absW = std::abs(position.w());
                        clipFlags[vIdx] = std::abs(position.x()) > absW || std::abs(position.y()) > absW || std::abs(position.z()) > absW;

                        // Perspective division
                        Math::Vector4D device = position / position.w();

                        // Apply viewport
                        Math::Vector4D preciseFragCoord = viewport * device;

                        // Form fragment coordinates as (x, y, z, 1/w)
                        preciseFragCoord.w() = 1 / position.w();

                        preciseFragCoords[vIdx] = preciseFragCoord;
                        varyings[vIdx] = varying;
                    }, vIdx);

                    if (threads.size() >= maxThreads) {
                        for (auto& thread: threads) {
                            thread.join();
                        }
                        threads.clear();
                    }
                }

                for (auto& thread: threads) {
                    thread.join();
                }
                threads.clear();

                // Primitive assembly: triangle primitives
                for (std::size_t tIdx = 0; tIdx < triangles.size(); tIdx++) {
                    threads.emplace_back([&varyings, &triangles, &preciseFragCoords, &clipFlags, &fBuffer, &zBuffer, &fragmentShader, &interpolator](std::size_t tIdx){
                        const auto& triangle = triangles[tIdx];
                        bool clipped = false;
                        std::array<V, 3> varyingGroup;
                        std::array<Math::Vector4D , 3> preciseFragCoordGroup;
                        for (std::size_t idx = 0; idx < 3; idx++) {
                            varyingGroup[idx] = varyings[triangle[idx]];
                            preciseFragCoordGroup[idx] = preciseFragCoords[triangle[idx]];
                            if (clipFlags[triangle[idx]]) {
                                clipped = true;
                                break;
                            }
                        }

                        // Rasterizing the primitive (fragment shader is also called)
                        if (!clipped) {
                            Rendering::Rasterizing::Triangle<V>(fBuffer, zBuffer, preciseFragCoordGroup, varyingGroup, fragmentShader, interpolator);
                        }
                    }, tIdx);

                    if (threads.size() >= maxThreads) {
                        for (auto& thread: threads) {
                            thread.join();
                        }
                        threads.clear();
                    }
                }

                for (auto& thread: threads) {
                    thread.join();
                }

                // Postprocessing
                postprocessor(fBuffer, zBuffer);
            }
        } // Rasterizing

    } // Rendering
} // UltRenderer

#endif //ULTRENDERER_PIPELINE_H
