//
// Created by ultcrt on 24-1-23.
//

#include "rendering/raytracing/Camera.h"

namespace UltRenderer {
    namespace Rendering {
        namespace Raytracing {
            Camera::Camera(double width, double height, double zMin, double zMax, ProjectionType projectionType):
                    ICamera(width, height, zMin, zMax, projectionType) {
            }

            Data::Image Rendering::Raytracing::Camera::render(std::size_t width, std::size_t height) const {
                return Data::Image(0, 0, Data::ImageFormat::RGBA);
            }
        } // Raytracing
    } // Rendering
} // UltRenderer