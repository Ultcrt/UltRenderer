//
// Created by ultcrt on 24-2-5.
//

#include "utils/Random.h"

namespace UltRenderer {
    namespace Utils {
        namespace Random {
            std::vector<Math::Vector3D> SampleFromUnitSphere(std::size_t n, std::size_t seed) {
                std::mt19937 mt(seed);
                // Tips: Use std::nextafter to random pick from [0, 1] instead of [0, 1)
                std::uniform_real_distribution<double> dist(0.0, std::nextafter(1.0, 2.0));

                // Uniformly picking from sphere, from: https://mathworld.wolfram.com/SpherePointPicking.html
                std::vector<Math::Vector3D> res;
                for (std::size_t idx = 0; idx < n; idx++) {
                    const double u = dist(mt);
                    const double v = dist(mt);
                    const double theta = 2 * M_PI * u;
                    const double phi = std::acos(2 * v - 1);

                    const double x = std::cos(theta) * std::sin(phi);
                    const double y = std::sin(theta) * std::sin(phi);
                    const double z = std::cos(phi);

                    res.emplace_back(x, y, z);
                }

                return res;
            }

            std::vector<Math::Vector3D> SampleFromUnitSemiSphere(std::size_t n, std::size_t seed) {
                std::mt19937 mt(seed);
                std::uniform_real_distribution<double> dist(0.0, std::nextafter(1.0, 2.0));

                std::vector<Math::Vector3D> res;
                for (std::size_t idx = 0; idx < n; idx++) {
                    const double u = dist(mt);
                    const double v = dist(mt);
                    // Only difference between this and SampleFromUnitSphere
                    const double theta = M_PI * u;
                    const double phi = std::acos(2 * v - 1);

                    const double x = std::sin(theta) * std::cos(phi);
                    const double y = std::sin(theta) * std::sin(phi);
                    const double z = std::cos(theta);

                    res.emplace_back(x, y, z);
                }

                return res;
            }
        }
    } // Utils
} // UltRenderer