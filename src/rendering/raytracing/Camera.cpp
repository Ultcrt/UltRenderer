//
// Created by ultcrt on 24-1-23.
//

#include <thread>
#include <iostream>
#include "rendering/raytracing/Camera.h"
#include "math/Ray.h"
#include "rendering/raytracing/shaders/WhittedStyleRaytracingShader.h"

namespace UltRenderer {
    namespace Rendering {
        namespace Raytracing {
            Camera::Camera(double width, double height, double zMin, double zMax, ProjectionType projectionType):
                    ICamera(width, height, zMin, zMax, projectionType) {
            }

            Data::Image Camera::render(std::size_t width, std::size_t height) const {
                Shaders::WhittedStyleRaytracingShader shader;
                return this->render<Shaders::WhittedStyleRaytracingShader>(width, height, shader);
            }
        } // Raytracing
    } // Rendering
} // UltRenderer