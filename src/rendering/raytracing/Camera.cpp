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
                Data::Image img(width, height, Data::ImageFormat::RGBA);

                const Math::Transform3D view = transformMatrix.inverse();

                // Raytracing is done under camera frame
                const Math::Vector3D origin = {0, 0, 0};

                for (std::size_t w = 0; w < width; w++) {
                    for (std::size_t h = 0; h < height; h++) {
                        const auto direction = Math::Vector3D {
                            ((static_cast<double>(w) + 0.5) / static_cast<double>(width)) * _width - _width / 2,
                            ((static_cast<double>(h) + 0.5) / static_cast<double>(height)) * _height - _height / 2,
                            -_zMax
                        }.normalized();
                    }
                }
            }
        } // Raytracing
    } // Rendering
} // UltRenderer