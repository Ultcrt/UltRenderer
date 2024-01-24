//
// Created by ultcrt on 24-1-22.
//

#ifndef ULTRENDERER_RAY_H
#define ULTRENDERER_RAY_H

#include "data/IntersectionInfo.h"
#include "data/TriangleMesh.h"
#include "math/Matrix.h"
#include "rendering/Scene.h"

namespace UltRenderer {
    namespace Math {
        class Ray {
        public:
            Vector3D origin;
            Vector3D direction;

            Ray(const Vector3D& ori, const Vector3D& dir);

            // TODO: Should implement intersect more than once
            [[nodiscard]] Data::TriangleIntersectionInfo intersect(const Data::TriangleMesh& mesh, bool fastCheck = false, double eps = 1e-5) const;
            [[nodiscard]] Data::TriangleIntersectionInfo intersect(const Rendering::Scene& scene, bool fastCheck = false, double eps = 1e-5) const;
            [[nodiscard]] Data::TriangleIntersectionInfo intersect(const std::vector<std::shared_ptr<Data::TriangleMesh>>& pMeshes, bool fastCheck = false, double eps = 1e-5) const;
            [[nodiscard]] Data::TriangleIntersectionInfo intersect(const Vector3D& p0, const Vector3D& p1, const Vector3D& p2, double eps = 1e-5) const;
        };
    } // Math
} // UltRenderer

#endif //ULTRENDERER_RAY_H
