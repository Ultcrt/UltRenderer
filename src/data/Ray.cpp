//
// Created by ultcrt on 24-1-22.
//

#include <iostream>
#include <algorithm>
#include "data/Ray.h"

namespace UltRenderer {
    namespace Data {
        Ray::Ray(const Math::Vector3D &ori, const Math::Vector3D &dir) : origin(ori), direction(dir.normalized()) {}

        Data::IntersectionInfo Ray::intersect(const Rendering::Scene &scene, bool fastCheck, double eps) const {
            // TODO: Should add more basic geometry
            Data::IntersectionInfo res;

            for (const auto& pNode: scene.intersectables()) {
                const auto info = pNode->intersect(*this, eps);

                if (info.isIntersected) {
                    if (info.length < res.length) {
                        res = info;
                        if (fastCheck) return res;
                    }
                }
            }

            return res;
        }


        Data::IntersectionInfo Ray::intersect(const Data::BoundingInfo &info, double eps) const {
            std::array<double, 3> tMins{};
            std::array<double, 3> tMaxs{};
            for (std::size_t idx = 0; idx < 3; idx++) {
                double t0 = (info.min[idx] - origin[idx]) / direction[idx];
                double t1 = (info.max[idx] - origin[idx]) / direction[idx];

                if (t0 <= t1) {
                    tMins[idx] = t0;
                    tMaxs[idx] = t1;
                }
                else {
                    tMins[idx] = t1;
                    tMaxs[idx] = t0;
                }
            }

            double enter = *std::max_element(tMins.begin(), tMins.end());
            double exit = *std::min_element(tMaxs.begin(), tMaxs.end());

            Data::IntersectionInfo res;
            if (exit >= 0 && exit >= enter) {
                res.isIntersected = true;
                res.length = enter;
            }
            return res;
        }

        Data::IntersectionInfo Ray::intersect(Hierarchy::IntersectableNode &node, double eps) const {
            return node.intersect(*this, eps);
        }
    } // Math
} // UltRenderer