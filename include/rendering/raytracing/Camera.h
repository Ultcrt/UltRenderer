//
// Created by ultcrt on 24-1-23.
//

#ifndef ULTRENDERER_RAYTRACING_CAMERA_H
#define ULTRENDERER_RAYTRACING_CAMERA_H

#include "rendering/ICamera.h"

namespace UltRenderer {
    namespace Rendering {
        namespace Raytracing {
            class Camera: public ICamera {
            public:
                Camera(double width, double height,
                       double zMin = 0.1, double zMax = 10, ProjectionType projectionType = ProjectionType::PERSPECTIVE);

                [[nodiscard]] Data::Image render(std::size_t width, std::size_t height) const override;
            };
        } // Raytracing
    } // Rendering
} // UltRenderer

#endif //ULTRENDERER_RAYTRACING_CAMERA_H
