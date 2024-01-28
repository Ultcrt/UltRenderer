//
// Created by ultcrt on 24-1-25.
//

#include "math/BVH.h"
#include "utils/Algorithm.h"
#include "math/Geometry.h"

namespace UltRenderer {
    namespace Math {
        namespace BVH {
            Tree::Tree(const std::vector<Math::Vector3D>& vertices, const std::vector<Math::Vector3S>& triangles, std::size_t maxLeafCapacity) {
                std::vector<Vector3D> midPoints;

                for (const auto& triangle: triangles) {
                    const auto& v0 = vertices[triangle[0]];
                    const auto& v1 = vertices[triangle[1]];
                    const auto& v2 = vertices[triangle[2]];

                    midPoints.emplace_back((v0 + v1 + v2) / 3);
                }

                std::vector<std::size_t> indices(midPoints.size());
                std::iota(indices.begin(), indices.end(), 0);

                pRoot = _create(vertices, triangles, midPoints, indices, maxLeafCapacity);
            }

            std::unique_ptr<Node> Tree::_create(const std::vector<Math::Vector3D>& vertices, const std::vector<Math::Vector3S>& triangles, const std::vector<Math::Vector3D> &points, std::vector<std::size_t>& indices, std::size_t maxLeafCapacity) {
                // Convert triangle indices to vertex ones
                std::vector<std::size_t> vertexIndices;
                for (const auto& tIdx: indices) {
                    vertexIndices.emplace_back(triangles[tIdx][0]);
                    vertexIndices.emplace_back(triangles[tIdx][1]);
                    vertexIndices.emplace_back(triangles[tIdx][2]);
                }
                Data::BoundingInfo boundingInfo = Math::Geometry::GetAABB(vertices, vertexIndices);
                std::size_t longestAxisIndex = Utils::ArgMax<double>({boundingInfo.width(), boundingInfo.height(), boundingInfo.depth()});

                std::unique_ptr<Node> pLeft = nullptr;
                std::unique_ptr<Node> pRight = nullptr;

                std::vector<std::size_t> primitiveIndices = {};

                // Need splitting
                if (indices.size() > maxLeafCapacity) {
                    const auto& begin = indices.begin();
                    const auto& mid = indices.begin() + static_cast<long>(indices.size()) / 2;
                    const auto& end = indices.end();

                    // Quick selection based on longest direction
                    std::nth_element(begin, mid, end, [&points, &longestAxisIndex](std::size_t a, std::size_t b){return points[a][longestAxisIndex] < points[b][longestAxisIndex];});

                    std::vector<std::size_t> leftIndices = {begin, mid};
                    std::vector<std::size_t> rightIndices = {mid, end};

                    pLeft = Tree::_create(vertices, triangles, points, leftIndices, maxLeafCapacity);
                    pRight = Tree::_create(vertices, triangles, points, rightIndices, maxLeafCapacity);
                }
                    // Leaf node, assign primitives. Only leaf node has not empty primitives
                else {
                    primitiveIndices = indices;
                }

                return std::make_unique<Node>(boundingInfo, std::move(pLeft), std::move(pRight), primitiveIndices);
            }

            Tree::Tree() {}

            Tree::Tree(const Tree &src): pRoot(std::make_unique<Node>(*src.pRoot)) {}

            Tree &Tree::operator=(const Tree &src) {
                pRoot = std::make_unique<Node>(*src.pRoot);

                return *this;
            }

            Node::Node(
                    const Data::BoundingInfo &info,
                    std::unique_ptr<Node> pl, std::unique_ptr<Node> pr,
                    const std::vector<std::size_t> &indices
            ): boundingInfo(info), pLeft(std::move(pl)), pRight(std::move(pr)), primitiveIndices(indices) {}

            Node::Node(const Node &src) {
                boundingInfo = src.boundingInfo;
                primitiveIndices = src.primitiveIndices;

                if (src.pLeft == nullptr and src.pRight == nullptr) {
                    pLeft = nullptr;
                    pRight = nullptr;
                }
                else {
                    pLeft = std::make_unique<Node>(*src.pLeft);
                    pRight = std::make_unique<Node>(*src.pRight);
                }
            }

            Node::Node(): boundingInfo(), primitiveIndices(), pLeft(), pRight() {}
        }
    } // Math
} // UltRenderer