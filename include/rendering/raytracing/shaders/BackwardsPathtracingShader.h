//
// Created by ultcrt on 24-2-4.
//

#ifndef ULTRENDERER_BACKWARDSPATHTRACINGSHADER_H
#define ULTRENDERER_BACKWARDSPATHTRACINGSHADER_H

#include "IRayShader.h"

namespace UltRenderer {
    namespace Rendering {
        namespace Raytracing {
            namespace Shaders {
                class BackwardsPathtracingShader: public IRayShader {
                public:
                    Data::Color<Data::ColorFormat::RGBA> operator()(
                            const Math::Vector3D& pixelCenterCamera,
                            double pixelWidthCamera,
                            double pixelHeightCamera,
                            const Math::Transform3D& transform,
                            const Math::Vector3D& cameraOriginWorld,
                            const Scene* pScene
                    ) const override;
                };
            } // Shaders
        } // Raytracing
    } // Rendering
} // UltRenderer

#endif //ULTRENDERER_BACKWARDSPATHTRACINGSHADER_H
