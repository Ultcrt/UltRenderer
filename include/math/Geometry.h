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

            template<typename T, std::size_t N>
            std::pair<VectorX<T, N>, VectorX<T, N>> GetMinMax(const std::vector<VectorX<T, N>>& points, const std::vector<std::size_t>& indices);

            Data::BoundingInfo GetAABB(const std::vector<Vector3D>& points);

            Data::BoundingInfo GetAABB(const std::vector<Vector3D>& points, const std::vector<std::size_t>& indices);

            Vector3D ComputeBarycentricCoords(const Vector2D& point, const std::array<Vector2D, 3>& trianglePoints);

            Vector3D ComputeBarycentricCoords(const Vector3D& point, const std::array<Vector3D, 3>& trianglePoints);

            std::vector<Vector3D> SampleFromUnitSphere(std::size_t n, std::size_t seed = std::random_device()());

            Vector3D ConvertDarbouxNormalToGlobal(const Vector3D& tangent, const Vector3D& normal, const Vector3D& target);

            Vector3D ComputeReflectionDirection(const Vector3D& normal, const Vector3D& direction);

            Vector3D ComputeRefractionDirection(const Vector3D& normal, const Vector3D& direction, double n12);

            double ComputeFresnel(const Vector3D& normal, const Vector3D& direction, double n1, double n2);

            /*----------Definition----------*/
            template<typename T, std::size_t N>
            std::pair<VectorX<T, N>, VectorX<T, N>>
            GetMinMax(const std::vector<VectorX<T, N>> &points) {
                VectorX<T, N> minVec;
                VectorX<T, N> maxVec;

                if (!points.empty()) {
                    minVec = points[0];
                    maxVec = points[0];

                    for (const auto point: points) {
                        for (std::size_t channelIdx = 0; channelIdx < N; channelIdx++) {
                            if (minVec[channelIdx] > point[channelIdx]) {
                                minVec[channelIdx] = point[channelIdx];
                            }
                            else if (maxVec[channelIdx] < point[channelIdx]) {
                                maxVec[channelIdx] = point[channelIdx];
                            }
                        }
                    }
                }

                return {minVec, maxVec};
            }

            template<typename T, std::size_t N>
            std::pair<VectorX<T, N>, VectorX<T, N>>
            GetMinMax(const std::vector<VectorX<T, N>> &points, const std::vector<std::size_t>& indices) {
                VectorX<T, N> minVec;
                VectorX<T, N> maxVec;

                if (!points.empty()) {
                    minVec = points[0];
                    maxVec = points[0];

                    for (const auto index: indices) {
                        for (std::size_t channelIdx = 0; channelIdx < N; channelIdx++) {
                            if (minVec[channelIdx] > points[index][channelIdx]) {
                                minVec[channelIdx] = points[index][channelIdx];
                            }
                            else if (maxVec[channelIdx] < points[index][channelIdx]) {
                                maxVec[channelIdx] = points[index][channelIdx];
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
