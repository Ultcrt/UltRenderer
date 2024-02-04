//
// Created by ultcrt on 24-2-4.
//

#include "rendering/raytracing/BackwardsPathtracingShader.h"

namespace UltRenderer {
    namespace Rendering {
        namespace Raytracing {
            Data::Color<Data::ColorFormat::RGBA>
            BackwardsPathtracingShader::operator()(const Math::Vector3D &pixelCenterCamera, double pixelWidthCamera,
                                                   double pixelHeightCamera, const Math::Transform3D &transform,
                                                   const Math::Vector3D &cameraOriginWorld, const Scene *pScene) const {

            }
        } // Raytracing
    } // Rendering
} // UltRenderer