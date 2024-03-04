//
// Created by ultcrt on 24-1-23.
//

#include <thread>
#include <iostream>
#include "rendering/raytracing/Camera.h"
#include "data/Ray.h"
#include "rendering/raytracing/shaders/WhittedStyleRaytracingShader.h"

namespace UltRenderer {
    namespace Rendering {
        namespace Raytracing {
            Camera::Camera(double width, double height, double zMin, double zMax, ProjectionType projectionType):
                    ICamera(width, height, zMin, zMax, projectionType) {
            }

            Data::Image Camera::render(std::size_t width, std::size_t height) const {
                Shaders::WhittedStyleRaytracingShader shader;
                return this->render(width, height, shader);
            }

            Data::Image Camera::render(std::size_t width, std::size_t height, const Shaders::IRayShader& rayShader) const {
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