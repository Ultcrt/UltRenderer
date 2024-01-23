//
// Created by ultcrt on 24-1-22.
//

#include "math/Ray.h"

namespace UltRenderer {
    namespace Math {
        Ray::Ray(const Vector3D &ori, const Vector3D &dir) : origin(ori), direction(dir.normalized()) {}

        Data::TriangleIntersectionInfo Ray::intersect(const Data::TriangleMesh &mesh, bool fastCheck, double eps) const {
            Data::TriangleIntersectionInfo res;
            for (std::size_t idx = 0; idx < mesh.triangles.size(); idx++) {
                const auto triangle = mesh.triangles[idx];

                // To world frame
                const auto p0 = (mesh.transformMatrix * mesh.vertices[triangle[0]].toHomogeneousCoordinates(1)).toCartesianCoordinates();
                const auto p1 = (mesh.transformMatrix * mesh.vertices[triangle[1]].toHomogeneousCoordinates(1)).toCartesianCoordinates();
                const auto p2 = (mesh.transformMatrix * mesh.vertices[triangle[2]].toHomogeneousCoordinates(1)).toCartesianCoordinates();

                const auto info = intersect(p0, p1, p2);

                if (info.isIntersected) {
                    if (info.length < res.length) {
                        res = info;
                        res.pMesh = &mesh;
                        res.triangleIdx = idx;
                        if (fastCheck) return res;
                    }
                }
            }
            return res;
        }

        Data::TriangleIntersectionInfo Ray::intersect(const Vector3D &p0, const Vector3D &p1, const Vector3D &p2, double eps) const {
            // Tips: Möller–Trumbore intersection algorithm
            const auto e1 = p1 - p0;
            const auto e2 = p2 - p0;
            const auto s = origin - p0;
            const auto s1 = direction.cross(e2);
            const auto s2 = s.cross(e1);

            const auto factor = s1.dot(e1);
            const auto t = s2.dot(e2) / factor;
            const auto b1 = s1.dot(s) / factor;
            const auto b2 = s2.dot(direction) / factor;

            Data::TriangleIntersectionInfo res;

            if (t >= 0 && b1 >= 0 && b2 >= 0 && 1 - b1 - b2 >= 0) {
                res.isIntersected = true;
                res.length = t;
                res.barycentricCoord = {1 - b1 - b2, b1, b2};
            }

            return res;
        }

        Data::TriangleIntersectionInfo Ray::intersect(const Rendering::Scene &scene, bool fastCheck, double eps) const {
            Data::TriangleIntersectionInfo res;

            for (const auto& pMesh: scene.meshes()) {
                const auto info = intersect(*pMesh, fastCheck, eps);

                if (info.isIntersected) {
                    if (info.length < res.length) {
                        res = info;
                        if (fastCheck) return res;
                    }
                }
            }

            return res;
        }
    } // Math
} // UltRenderer