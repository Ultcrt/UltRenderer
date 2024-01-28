//
// Created by ultcrt on 24-1-25.
//

#ifndef ULTRENDERER_BVH_H
#define ULTRENDERER_BVH_H

#include <vector>
#include <memory>
#include "data/BoundingInfo.h"

namespace UltRenderer {
    namespace Math {
        namespace BVH {
            struct Node {
                Data::BoundingInfo boundingInfo;
                std::unique_ptr<Node> pLeft;
                std::unique_ptr<Node> pRight;
                std::vector<std::size_t> primitiveIndices;

                Node();
                Node(const Data::BoundingInfo& info, std::unique_ptr<Node> pl, std::unique_ptr<Node> pr, const std::vector<std::size_t>& indices);
                Node(const Node& src);
            };

            // A simple implement of binary Tree
            class Tree {
            private:
                std::unique_ptr<Node> _create(const std::vector<Math::Vector3D>& vertices, const std::vector<Math::Vector3S>& triangles, const std::vector<Math::Vector3D>& points, std::vector<std::size_t>& indices, std::size_t maxLeafCapacity);

            public:
                explicit Tree(const std::vector<Math::Vector3D>& vertices, const std::vector<Math::Vector3S>& triangles, std::size_t maxLeafCapacity = 5);
                Tree();
                Tree(const Tree& src);
                Tree& operator=(const Tree& src);

                std::unique_ptr<Node> pRoot;
            };
        }
    } // Math
} // UltRenderer

#endif //ULTRENDERER_BVH_H
