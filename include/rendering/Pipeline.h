//
// Created by ultcrt on 24-1-6.
//

#ifndef ULTRENDERER_PIPELINE_H
#define ULTRENDERER_PIPELINE_H

#include <vector>
#include <concepts>
#include "math/Matrix.h"
#include "data/Image.h"
#include "shaders/IShader.h"
#include "rendering/Rasterize.h"
#include "math/Transform.h"

namespace UltRenderer {
    namespace Rendering {
        class Pipeline {
        public:
            template <std::derived_from<Shaders::IVarying> V>
            static void Execute(Data::Image& fBuffer, Data::Image& zBuffer, const Math::Transform3D& viewport, std::size_t vertexNum,
                                const std::vector<Math::Vector3S>& triangles, const std::vector<Math::Vector2S>& lines, const std::vector<std::size_t>& points,
                                const Shaders::IVertexShader<V>& vertexShader,
                                const Shaders::IFragmentShader<V>& fragmentShader,
                                const Shaders::IInterpolator<V>& interpolator = {});
        };

        // TODO: Only support triangle primitives for now
        template <std::derived_from<Shaders::IVarying> V>
        void Pipeline::Execute(Data::Image& fBuffer, Data::Image& zBuffer, const Math::Transform3D& viewport, std::size_t vertexNum,
                               const std::vector<Math::Vector3S>& triangles, const std::vector<Math::Vector2S>& lines, const std::vector<std::size_t>& points,
                               const Shaders::IVertexShader<V>& vertexShader,
                               const Shaders::IFragmentShader<V>& fragmentShader,
                               const Shaders::IInterpolator<V>& interpolator) {
            // Process vertex
            std::vector<V> varyings;
            std::vector<bool> clipFlags;
            for (std::size_t vIdx = 0; vIdx < vertexNum; vIdx++) {
                // Call vertex shader for each vertex
                V varying = vertexShader(vIdx);

                // Record vertex clip flag here
                const double w = std::abs(varying.position.w());
                clipFlags.emplace_back(std::abs(varying.position.x()) > w || std::abs(varying.position.y()) > w || std::abs(varying.position.z()) > w);

                // Apply viewport here
                varying.position = viewport * varying.position / varying.position.w();
                varyings.emplace_back(varying);
            }

            // Primitive assembly: triangle primitives
            for (const auto& triangle: triangles) {
                bool clipped = false;
                std::array<V, 3> varyingGroup;
                // TODO: Perspective correcting is not done, and clipping is not ideal (discard triangle when one vertex is outside)
                for (std::size_t idx = 0; idx < 3; idx++) {
                    varyingGroup[idx] = varyings[triangle[idx]];
                    if (clipFlags[triangle[idx]]) {
                        clipped = true;
                        break;
                    }
                }

                // Rasterize the primitive (fragment shader is also called)
                if (!clipped) {
                    Rendering::Rasterize::Triangle<V>(fBuffer, zBuffer, varyingGroup, fragmentShader, interpolator);
                }
            }
        }
    } // Rendering
} // UltRenderer

#endif //ULTRENDERER_PIPELINE_H
