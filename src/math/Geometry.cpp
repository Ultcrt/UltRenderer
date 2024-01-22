//
// Created by ultcrt on 23-12-14.
//

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
        }
    } // Utils
} // UltRender