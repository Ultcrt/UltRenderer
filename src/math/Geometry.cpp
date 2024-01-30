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

            Data::BoundingInfo GetAABB(const std::vector<Vector3D>& points, const std::vector<std::size_t>& indices) {
                auto [min, max] = GetMinMax<double, 3>(points, indices);
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

            Vector3D ConvertDarbouxNormalToGlobal(const Vector3D& tangent, const Vector3D& normal, const Vector3D& target) {
                // Make sure TBN are orthogonal
                auto b = normal.cross(tangent);
                auto n = normal;
                auto t = b.cross(n);

                t.normalize();
                b.normalize();
                n.normalize();

                // Just a rotation matrix of TBN basis
                Math::Matrix3D tbn = {
                        t.x(), b.x(), n.x(),
                        t.y(), b.y(), n.y(),
                        t.z(), b.z(), n.z(),
                };

                return (tbn * target).normalized();
            }

            Vector3D ComputeReflectionDirection(const Vector3D& normal, const Vector3D& direction) {
                return (-2 * direction.dot(normal) * normal + direction).normalized();
            }

            Vector3D ComputeRefractionDirection(const Vector3D& normal, const Vector3D& direction, double n12) {
                Vector3D n = normal;
                double cos1 = - n.dot(direction);

                if (cos1 < 0) {
                    n = -normal;
                    cos1 = -cos1;
                }

                // From https://en.wikipedia.org/wiki/Snell%27s_law
                return (n12 * direction + (n12 * cos1 - std::sqrt(1 - std::pow(n12, 2) * (1 - std::pow(cos1, 2)))) * n).normalized();
            }

            double ComputeFresnel(const Vector3D& normal, const Vector3D& direction, double n1, double n2) {
                double n12 = n1 / n2;
                Vector3D n = normal;
                double cos1 = - n.dot(direction);

                if (cos1 < 0) {
                    n = -normal;
                    cos1 = -cos1;
                }

                double sin1 = std::sqrt(1 - std::pow(cos1, 2));
                double sin2 = n12 * sin1;
                double cos2 = std::sqrt(1 - std::pow(cos2, 2));

                double rs = std::pow((n1 * cos1 - n2 * cos2) / (n1 * cos1 + n2 * cos2), 2);
                double rp = std::pow((n1 * cos2 - n2 * cos1) / (n1 * cos2 + n2 * cos1), 2);

                return (rs + rp) / 2;
            }
        }
    } // Utils
} // UltRender