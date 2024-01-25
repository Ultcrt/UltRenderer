//
// Created by ultcrt on 24-1-25.
//

#ifndef ULTRENDERER_BVH_H
#define ULTRENDERER_BVH_H

#include <vector>
#include "data/TriangleMesh.h"
#include "data/BoundingInfo.h"

namespace UltRenderer {
    namespace Math {
        // A simple implement of binary BVH
        class BVH {
        private:
            class Node {
                Data::BoundingInfo boundingInfo;
                Node* pLeft;
                Node* pRight;
                std::vector<std::size_t> primitiveIndices;
            };

        public:
            explicit BVH(const Data::TriangleMesh& mesh);
        };
    } // Math
} // UltRenderer

#endif //ULTRENDERER_BVH_H
