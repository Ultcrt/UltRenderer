//
// Created by ultcrt on 24-1-23.
//

#ifndef ULTRENDERER_RAYTRACING_CAMERA_H
#define ULTRENDERER_RAYTRACING_CAMERA_H

#include <thread>
#include "rendering/ICamera.h"
#include "rendering/raytracing/shaders/WhittedStyleRaytracingShader.h"

namespace UltRenderer {
    namespace Rendering {
        namespace Raytracing {
            class Camera: public ICamera {
            public:
                std::size_t maxThreads = 20;

                // TODO: Need to deal with ORTHOGONAL projection
                Camera(double width, double height,
                       double zMin = 0.1, double zMax = 10, ProjectionType projectionType = ProjectionType::PERSPECTIVE);

                [[nodiscard]] Data::Image render(std::size_t width, std::size_t height, const Shaders::IRayShader& rayShader) const;

                [[nodiscard]] Data::Image render(std::size_t width, std::size_t height) const override;
            };
        } // Raytracing
    } // Rendering
} // UltRenderer

#endif //ULTRENDERER_RAYTRACING_CAMERA_H
