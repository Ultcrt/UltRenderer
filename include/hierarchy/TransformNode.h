//
// Created by ultcrt on 24-1-2.
//

#ifndef ULTRENDERER_TRANSFORMNODE_H
#define ULTRENDERER_TRANSFORMNODE_H

#include "Node.h"
#include "math/Transform.h"

namespace UltRenderer {
    namespace Hierarchy {
        class TransformNode: public Node {
        public:
            Math::Transform3D transformMatrix;
        };
    } // Hierarchy
} // UltRenderer

#endif //ULTRENDERER_TRANSFORMNODE_H
