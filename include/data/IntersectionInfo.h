//
// Created by ultcrt on 24-1-22.
//

#ifndef ULTRENDERER_INTERSECTIONINFO_H
#define ULTRENDERER_INTERSECTIONINFO_H

#include "math/Matrix.h"

namespace UltRenderer {
    namespace Data {
        struct IntersectionInfo {
            bool isIntersected = false;
            double length = std::numeric_limits<double>::infinity();
            Math::Vector3D intersectedPoint = {std::nan(""), std::nan(""), std::nan("")};
        };
    } // Data
} // UltRenderer

#endif //ULTRENDERER_INTERSECTIONINFO_H
