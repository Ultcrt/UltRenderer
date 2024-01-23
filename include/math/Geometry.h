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
#include "data/BoundingInfo.h"

namespace UltRenderer {
    namespace Math {
        namespace Geometry {
            /*----------Declaration----------*/
            template<typename T, std::size_t N>
            std::pair<VectorX<T, N>, VectorX<T, N>> GetMinMax(const std::vector<VectorX<T, N>>& points);

            Data::BoundingInfo GetAABB(const std::vector<Vector3D>& points);

            Vector3D ComputeBarycentricCoords(const Vector2D& point, const std::array<Vector2D, 3>& trianglePoints);

            Vector3D ComputeBarycentricCoords(const Vector3D& point, const std::array<Vector3D, 3>& trianglePoints);

            std::vector<Vector3D> SampleFromUnitSphere(std::size_t n, std::size_t seed = std::random_device()());

            /*----------Definition----------*/
            template<typename T, std::size_t N>
            std::pair<VectorX<T, N>, VectorX<T, N>>
            GetMinMax(const std::vector<VectorX<T, N>> &points) {
                VectorX<T, N> minVec;
                VectorX<T, N> maxVec;

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
        }
    } // Utils
} // UltRender

#endif //ULTRENDERER_GEOMETRY_H
