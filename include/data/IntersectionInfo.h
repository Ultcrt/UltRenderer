//
// Created by ultcrt on 24-1-22.
//

#ifndef ULTRENDERER_INTERSECTIONINFO_H
#define ULTRENDERER_INTERSECTIONINFO_H

#include "math/Matrix.h"
#include "data/Image.h"
#include "hierarchy/IntersectableNode.h"

namespace UltRenderer {
    namespace Data {
        struct IntersectionInfo {
            bool isIntersected = false;
            double length = std::numeric_limits<double>::infinity();
            Hierarchy::IntersectableNode* pNode = nullptr;
            Math::Vector3D uv = {0, 0};
            Math::Vector3D normal = {0, 1, 0};

            IntersectionInfo& operator=(const IntersectionInfo& target);
        };
    } // Data
} // UltRenderer

#endif //ULTRENDERER_INTERSECTIONINFO_H
