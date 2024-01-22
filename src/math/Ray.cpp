//
// Created by ultcrt on 24-1-22.
//

#include "math/Ray.h"

namespace UltRenderer {
    namespace Math {
        Ray::Ray(const Vector3D &ori, const Vector3D &dir) : origin(ori), direction(dir.normalized()) {}

        Data::IntersectionInfo Ray::intersect(const Data::TriangleMesh &mesh, bool fastCheck, double eps) const {
            Data::IntersectionInfo res;
            for (const auto triangle: mesh.triangles) {
                const auto info = intersect(mesh.vertices[triangle[0]], mesh.vertices[triangle[1]],mesh.vertices[triangle[2]]);

                if (info.isIntersected) {
                    if (info.length < res.length) {
                        res = info;
                        if (fastCheck) return res;
                    }
                }
            }
            return res;
        }

        Data::IntersectionInfo Ray::intersect(const Vector3D &p0, const Vector3D &p1, const Vector3D &p2, double eps) const {
            // Tips: Möller–Trumbore intersection algorithm
            const auto e1 = p1 - p0;
            const auto e2 = p2 - p0;
            const auto s = origin - p0;
            const auto s1 = direction.cross(e2);
            const auto s2 = s.cross(e1);

            const auto factor = s1.dot(e1);
            const auto t = s2.dot(e2) / factor;
            const auto b1 = s1.dot(s) / factor;
            const auto b2 = s2.dot(direction);

            Data::IntersectionInfo res;

            if (t >= 0 && b1 >= 0 && b2 >= 0 && 1 - b1 - b2 >= 0) {
                res.isIntersected = true;
                res.length = t;
                res.intersectedPoint = origin + t * direction;
            }

            return res;
        }
    } // Math
} // UltRenderer