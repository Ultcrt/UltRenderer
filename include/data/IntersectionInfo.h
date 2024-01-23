//
// Created by ultcrt on 24-1-22.
//

#ifndef ULTRENDERER_INTERSECTIONINFO_H
#define ULTRENDERER_INTERSECTIONINFO_H

#include "math/Matrix.h"
#include "data/Image.h"
#include "data/TriangleMesh.h"

namespace UltRenderer {
    namespace Data {
        struct IntersectionInfo {
            bool isIntersected = false;
            double length = std::numeric_limits<double>::infinity();
        };

        struct TriangleIntersectionInfo: public IntersectionInfo {
            const Data::TriangleMesh* pMesh = nullptr;
            std::size_t triangleIdx = 0;
            Math::Vector3D barycentricCoord = {0, 0, 0};
        };
    } // Data
} // UltRenderer

#endif //ULTRENDERER_INTERSECTIONINFO_H
