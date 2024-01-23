//
// Created by ultcrt on 23-12-14.
//

#include <iostream>
#include "math/Geometry.h"

namespace UltRenderer {
    namespace Math {
        namespace Geometry {
            std::vector<Vector3D> SampleFromUnitSphere(std::size_t n, std::size_t seed) {
                std::mt19937 mt(seed);
                // Tips: Use std::nextafter to random pick from [0, 1] instead of [0, 1)
                std::uniform_real_distribution<double> dist(0.0, std::nextafter(1.0, 2.0));

                // Uniformly picking from sphere, from: https://mathworld.wolfram.com/SpherePointPicking.html
                std::vector<Vector3D> res;
                for (std::size_t idx = 0; idx < n; idx++) {
                    const double u = dist(mt);
                    const double v = dist(mt);
                    const double theta = 2 * M_PI * u;
                    const double phi = std::acos(2 * v - 1);

                    const double x = std::sin(theta) * std::cos(phi);
                    const double y = std::sin(theta) * std::sin(phi);
                    const double z = std::cos(theta);

                    res.emplace_back(x, y, z);
                }

                return res;
            }

            Data::BoundingInfo GetAABB(const std::vector<Vector3D>& points) {
                auto [min, max] = GetMinMax<double, 3>(points);
                return {min, max};
            }

            Vector3D ComputeBarycentricCoords(const Vector2D& point,
                                                  const std::array<Vector2D, 3>& trianglePoints) {
                Vector2D vecAB = trianglePoints[1] - trianglePoints[0];
                Vector2D vecAC = trianglePoints[2] - trianglePoints[0];
                Vector2D vecAP = point - trianglePoints[0];

                double triangleArea2 = vecAB.x() * vecAC.y() - vecAB.y() * vecAC.x();

                double v = (vecAP.x() * vecAC.y() - vecAP.y() * vecAC.x()) / triangleArea2;
                double w = (vecAB.x() * vecAP.y() - vecAB.y() * vecAP.x()) / triangleArea2;
                double u = 1 - v - w;

                return {u, v, w};
            }

            Vector3D ComputeBarycentricCoords(const Vector3D& point, const std::array<Vector3D, 3>& trianglePoints) {
                const Vector3D vecAB = trianglePoints[1] - trianglePoints[0];
                const Vector3D vecAC = trianglePoints[2] - trianglePoints[0];
                const Vector3D vecAP = point - trianglePoints[0];

                Vector3D normal = vecAB.cross(vecAC);
                const double triangleArea2 = normal.norm();
                normal /= triangleArea2;

                double v = vecAP.cross(vecAC).dot(normal) / triangleArea2;
                double w = vecAB.cross(vecAP).dot(normal) / triangleArea2;
                double u = 1 - v - w;

                return {u, v, w};
            }
        }
    } // Utils
} // UltRender