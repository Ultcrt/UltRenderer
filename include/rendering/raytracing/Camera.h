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

                template<std::derived_from<Shaders::IRayShader> RS>
                [[nodiscard]] Data::Image render(std::size_t width, std::size_t height, const RS& rayShader) const;

                [[nodiscard]] Data::Image render(std::size_t width, std::size_t height) const override;
            };

            template<std::derived_from<Shaders::IRayShader> RS>
            Data::Image Camera::render(std::size_t width, std::size_t height, const RS& rayShader) const {
                Data::Image img(width, height, Data::ColorFormat::RGBA);

                // Raytracing is done under world frame
                const Math::Vector3D origin = (transformMatrix * Math::Vector4D{0, 0, 0, 1}).toCartesianCoordinates();

                const double pixelWidthCamera = _width / static_cast<double>(width);
                const double pixelHeightCamera = _height / static_cast<double>(height);

                std::vector<std::thread> threads;
                for (std::size_t w = 0; w < width; w++) {
                    for (std::size_t h = 0; h < height; h++) {
                        // Tips: w, h need to be passed as param or capture by value instead of capture by reference, because capture by reference has competition problem
                        threads.emplace_back([this, pixelWidthCamera, pixelHeightCamera, &origin, &rayShader, &img](std::size_t w, std::size_t h){
                            const auto pixelCenterCamera = Math::Vector3D{
                                    (static_cast<double>(w) + 0.5) * pixelWidthCamera - _width / 2,
                                    (static_cast<double>(h) + 0.5) * pixelHeightCamera - _height / 2,
                                    -_zMin
                            };

                            img.at<Data::ColorFormat::RGBA>(w, h) = rayShader(pixelCenterCamera, pixelWidthCamera, pixelHeightCamera, transformMatrix, origin, _pScene);
                        }, w, h);

                        if (threads.size() >= maxThreads) {
                            for (auto& thread: threads) {
                                thread.join();
                            }
                            threads.clear();
                        }
                    }
                }

                for (auto& thread: threads) {
                    thread.join();
                }

                return img;
            }
        } // Raytracing
    } // Rendering
} // UltRenderer

#endif //ULTRENDERER_RAYTRACING_CAMERA_H
