//
// Created by ultcrt on 24-1-23.
//

#include <iostream>
#include "rendering/raytracing/Camera.h"
#include "math/Ray.h"
#include "rendering/raytracing/Raytracing.h"

namespace UltRenderer {
    namespace Rendering {
        namespace Raytracing {
            Camera::Camera(double width, double height, double zMin, double zMax, ProjectionType projectionType):
                    ICamera(width, height, zMin, zMax, projectionType) {
            }

            Data::Image Rendering::Raytracing::Camera::render(std::size_t width, std::size_t height) const {
                Data::Image img(width, height, Data::ImageFormat::RGBA);

                // Raytracing is done under world frame
                const Math::Vector3D origin = (transformMatrix * Math::Vector4D{0, 0, 0, 1}).toCartesianCoordinates();

                for (std::size_t w = 0; w < width; w++) {
                    for (std::size_t h = 0; h < height; h++) {
                        const auto fragCoord = (transformMatrix * Math::Vector4D {
                                ((static_cast<double>(w) + 0.5) / static_cast<double>(width)) * _width - _width / 2,
                                ((static_cast<double>(h) + 0.5) / static_cast<double>(height)) * _height - _height / 2,
                                -_zMin,
                                1
                        }).toCartesianCoordinates();

                        const Math::Ray ray(origin, (fragCoord - origin).normalized());

                        const auto color = Raytracing::Cast(ray, _pScene->meshes());

                        img.at<Data::ImageFormat::RGBA>(w, h) = color;
                    }
                }

                return img;
            }
        } // Raytracing
    } // Rendering
} // UltRenderer