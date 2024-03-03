//
// Created by ultcrt on 24-2-4.
//

#ifndef ULTRENDERER_BACKWARDSPATHTRACINGSHADER_H
#define ULTRENDERER_BACKWARDSPATHTRACINGSHADER_H

#include "IRayShader.h"
#include "data/Ray.h"

namespace UltRenderer {
    namespace Rendering {
        namespace Raytracing {
            namespace Shaders {
                class BackwardsPathtracingShader: public IRayShader {
                private:
                    const static Math::Matrix3D _transformY2Z;
                    const static double _uniformSamplingPossibility;

                public:
                    Math::Vector4D backgroundColor = {0, 0, 0, 1};
                    double eps = 1e-6;
                    double dropout = 0.3;
                    std::size_t numBouncedRays = 1;
                    std::size_t samplesPerPixel = 500;

                    Data::Color<Data::ColorFormat::RGBA> operator()(
                            const Math::Vector3D& pixelCenterCamera,
                            double pixelWidthCamera,
                            double pixelHeightCamera,
                            const Math::Transform3D& transform,
                            const Math::Vector3D& cameraOriginWorld,
                            const Scene* pScene
                    ) const override;

                    Data::Color<Data::ColorFormat::RGBA> Cast(
                            const Data::Ray& ray,
                            const Scene* pScene) const;
                };
            } // Shaders
        } // Raytracing
    } // Rendering
} // UltRenderer

#endif //ULTRENDERER_BACKWARDSPATHTRACINGSHADER_H
