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
    namespace Data {
        class Ray {
        public:
            Math::Vector3D origin;
            Math::Vector3D direction;

            Ray(const Math::Vector3D& ori, const Math::Vector3D& dir);

            // TODO: Should implement intersect more than once
            [[nodiscard]] Data::IntersectionInfo intersect(Hierarchy::IntersectableNode& node, double eps = 1e-5) const;
            [[nodiscard]] Data::IntersectionInfo intersect(const Rendering::Scene& scene, bool fastCheck = false, double eps = 1e-5) const;
            [[nodiscard]] Data::IntersectionInfo intersect(const Data::BoundingInfo& info, double eps = 1e-5) const;
        };
    } // Math
} // UltRenderer

#endif //ULTRENDERER_RAY_H
