//
// Created by ultcrt on 24-1-2.
//

#ifndef ULTRENDERER_LIGHT_H
#define ULTRENDERER_LIGHT_H

#include "math/Matrix.h"
#include "hierarchy/Node.h"

namespace UltRenderer {
    namespace Rendering {
        class Light: public Hierarchy::Node {
        public:
            Light(const Math::Vector3D& pos, double intensity = 1);

            Math::Vector3D position;
            double intensity;
        };
    } // Rendering
} // UltRenderer

#endif //ULTRENDERER_LIGHT_H
