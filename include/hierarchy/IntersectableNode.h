//
// Created by ultcrt on 24-2-21.
//

#ifndef ULTRENDERER_INTERSECTABLENODE_H
#define ULTRENDERER_INTERSECTABLENODE_H

#include "TransformNode.h"
#include "rendering/material/CommonMaterial.h"
#include "data/BoundingInfo.h"

namespace UltRenderer {
    namespace Data {
        struct IntersectionInfo;
        class Ray;
    }

    namespace Hierarchy {
        class IntersectableNode: public TransformNode {
        public:
            std::shared_ptr<Rendering::Material::CommonMaterial> pMaterial;
            Data::BoundingInfo boundingInfo;

            [[nodiscard]] virtual Data::IntersectionInfo intersect(const Data::Ray& ray, double eps) = 0;
        };
    } // Rendering
} // UltRenderer

#endif //ULTRENDERER_INTERSECTABLENODE_H
