//
// Created by ultcrt on 23-12-14.
//

#ifndef ULTRENDERER_GEOMETRY_H
#define ULTRENDERER_GEOMETRY_H

#include <random>
#include <utility>
#include <vector>
#include <array>
#include "Matrix.h"

namespace UltRenderer {
    namespace Math {
        namespace Geometry {
            /*----------Declaration----------*/
            template<typename T, std::size_t N>
            std::pair<Math::VectorX<T, N>, Math::VectorX<T, N>> GetAABB(const std::vector<Math::VectorX<T, N>>& points);

            template<typename T>
            Math::Vector3<T> ComputeBarycentricCoords2D(const Math::Vector2<T>& point, const std::array<Math::Vector2<T>, 3>& trianglePoints);

            std::vector<Math::Vector3D> SampleFromUnitSphere(std::size_t n, std::size_t seed = std::random_device()());

            std::pair<Math::Vector3D, double> ComputeApproximateBoundingSphere(const std::vector<Math::Vector3D>& vertices);

            /*----------Definition----------*/
            template<typename T, std::size_t N>
            std::pair<Math::VectorX<T, N>, Math::VectorX<T, N>>
            GetAABB(const std::vector<Math::VectorX<T, N>> &points) {
                Math::VectorX<T, N> minVec;
                Math::VectorX<T, N> maxVec;

                if (!points.empty()) {
                    minVec = points[0];
                    maxVec = points[0];

                    for (auto iter = points.begin() + 1; iter != points.end(); iter++) {
                        for (std::size_t channelIdx = 0; channelIdx < N; channelIdx++) {
                            if (minVec[channelIdx] > (*iter)[channelIdx]) {
                                minVec[channelIdx] = (*iter)[channelIdx];
                            }
                            else if (maxVec[channelIdx] < (*iter)[channelIdx]) {
                                maxVec[channelIdx] = (*iter)[channelIdx];
                            }
                        }
                    }
                }

                return {minVec, maxVec};
            }

            template<typename T>
            Math::Vector3<T> ComputeBarycentricCoords2D(const Math::Vector2<T>& point,
                                                        const std::array<Math::Vector2<T>, 3>& trianglePoints) {
                Math::Vector2<T> vecAB = trianglePoints[1] - trianglePoints[0];
                Math::Vector2<T> vecAC = trianglePoints[2] - trianglePoints[0];
                Math::Vector2<T> vecAP = point - trianglePoints[0];

                T triangleArea2 = vecAC.x() * vecAB.y() - vecAC.y() * vecAB.x();

                T v = (vecAC.x() * vecAP.y() - vecAC.y() * vecAP.x()) / triangleArea2;
                T w = (vecAP.x() * vecAB.y() - vecAP.y() * vecAB.x()) / triangleArea2;
                T u = 1 - v - w;

                return {u, v, w};
            }
        }
    } // Utils
} // UltRender

#endif //ULTRENDERER_GEOMETRY_H
