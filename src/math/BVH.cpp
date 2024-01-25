//
// Created by ultcrt on 24-1-25.
//

#include "math/BVH.h"
#include "utils/Algorithm.h"

namespace UltRenderer {
    namespace Math {
        BVH::BVH(const Data::TriangleMesh &mesh) {
            std::vector<Vector3D> midPoints;

            for (const auto& triangle: mesh.triangles) {
                const auto& v0 = mesh.vertices[triangle[0]];
                const auto& v1 = mesh.vertices[triangle[1]];
                const auto& v2 = mesh.vertices[triangle[2]];

                midPoints.emplace_back((v0 + v1 + v2) / 3);
            }

            auto sortedIndices = Utils::ArgSort(midPoints, [](const Vector3D& a, const Vector3D& b){return a.x() < b.x();});
        }
    } // Math
} // UltRenderer