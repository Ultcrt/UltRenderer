//
// Created by ultcrt on 24-1-22.
//

#ifndef ULTRENDERER_WHITTEDSTYLERAYTRACINGSHADER_H
#define ULTRENDERER_WHITTEDSTYLERAYTRACINGSHADER_H

#include <memory>
#include <vector>
#include "math/Matrix.h"
#include "math/Ray.h"
#include "data/TriangleMesh.h"
#include "IRayShader.h"

namespace UltRenderer {
    namespace Rendering {
        namespace Raytracing {
            class WhittedStyleRaytracingShader: public IRayShader {
            public:
                Data::Color<Data::ColorFormat::RGBA> operator()(
                        const Math::Vector3D& pixelCenterCamera,
                        double pixelWidthCamera,
                        double pixelHeightCamera,
                        const Math::Transform3D& transform,
                        const Math::Vector3D& cameraOriginWorld,
                        const Scene* pScene) const override;

                static Data::Color<Data::ColorFormat::RGBA> Cast(
                        const Math::Ray& ray,
                        const Scene* pScene,
                        const Math::Vector4D& backgroundColor = {1, 1, 1, 1},
                        double eps = 1e-6,
                        std::size_t maxRecursion = 3);
            };
        } // Raytracing
    } // Rendering
} // UltRenderer

#endif //ULTRENDERER_WHITTEDSTYLERAYTRACINGSHADER_H
