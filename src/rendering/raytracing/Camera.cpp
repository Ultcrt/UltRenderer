//
// Created by ultcrt on 24-1-23.
//

#include <thread>
#include <iostream>
#include "rendering/raytracing/Camera.h"
#include "math/Ray.h"
#include "rendering/raytracing/WhittedStyleRaytracing.h"

namespace UltRenderer {
    namespace Rendering {
        namespace Raytracing {
            Camera::Camera(double width, double height, double zMin, double zMax, ProjectionType projectionType):
                    ICamera(width, height, zMin, zMax, projectionType) {
            }

            Data::Image Rendering::Raytracing::Camera::render(std::size_t width, std::size_t height) const {
                Data::Image img(width, height, Data::ColorFormat::RGBA);

                // Raytracing is done under world frame
                const Math::Vector3D origin = (transformMatrix * Math::Vector4D{0, 0, 0, 1}).toCartesianCoordinates();

                std::vector<std::thread> threads;
                for (std::size_t w = 0; w < width; w++) {
                    for (std::size_t h = 0; h < height; h++) {
                        // Tips: w, h need to be passed as param or capture by value instead of capture by reference, because capture by reference has competition problem
                        threads.emplace_back([this, width, height, &origin, &img](std::size_t w, std::size_t h){
                            const auto fragCoord = (transformMatrix * Math::Vector4D {
                                    ((static_cast<double>(w) + 0.5) / static_cast<double>(width)) * _width - _width / 2,
                                    ((static_cast<double>(h) + 0.5) / static_cast<double>(height)) * _height - _height / 2,
                                    -_zMin,
                                    1
                            }).toCartesianCoordinates();

                            const Math::Ray ray(origin, (fragCoord - origin).normalized());

                            const auto color = Raytracing::Cast(ray, _pScene);

                            img.at<Data::ColorFormat::RGBA>(w, h) = color;
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