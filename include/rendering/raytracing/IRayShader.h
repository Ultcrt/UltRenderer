//
// Created by ultcrt on 24-2-4.
//

#ifndef ULTRENDERER_IRAYSHADER_H
#define ULTRENDERER_IRAYSHADER_H

#include "data/Color.h"
#include "rendering/Scene.h"

namespace UltRenderer {
    namespace Rendering {
        namespace Raytracing {
            class IRayShader {
            public:
                virtual Data::Color<Data::ColorFormat::RGBA> operator()(
                        const Math::Vector3D& pixelCenterCamera,
                        double pixelWidthCamera,
                        double pixelHeightCamera,
                        const Math::Transform3D& transform,
                        const Math::Vector3D& cameraOriginWorld,
                        const Scene* pScene
                ) const = 0;
            };
        } // Raytracing
    } // Rendering
} // UltRenderer

#endif //ULTRENDERER_IRAYSHADER_H
