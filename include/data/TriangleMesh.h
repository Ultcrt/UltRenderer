//
// Created by ultcrt on 23-11-23.
//

#ifndef ULTRENDERER_TRIANGLEMESH_H
#define ULTRENDERER_TRIANGLEMESH_H

#include <vector>
#include <string>
#include "math/Matrix.h"
#include <memory>
#include "data/Image.h"
#include "math/Transform.h"
#include "hierarchy/IntersectableNode.h"
#include "set"
#include "data/BoundingInfo.h"
#include "math/BVH.h"
#include "rendering/material/CommonMaterial.h"

namespace UltRenderer {
    namespace Data {
        class TriangleMesh: public Hierarchy::IntersectableNode {
        private:
            std::vector<Math::Vector3D> _transformedVertices;
            Data::IntersectionInfo
            _intersectBVH(const Data::Ray& ray, const Math::BVH::Node *pNode, bool& stop, bool fastCheck, double eps);

            Data::IntersectionInfo _intersectTriangle(const Data::Ray& ray, std::size_t triangleIdx, double eps);

        public:
            std::vector<Math::Vector3D> vertices;
            std::vector<Math::Vector3S> triangles;
            std::vector<Math::Vector3D> vertexColors;
            std::vector<Math::Vector3D> vertexNormals;
            std::vector<Math::Vector3D> vertexTangents;
            std::vector<Math::Vector3D> vertexTextures;
            std::vector<Math::Vector3D> triangleNormals;
            std::vector<Math::Vector3D> triangleTangents;
            std::vector<std::set<std::size_t>> adjacentVertices;
            std::vector<std::set<std::size_t>> adjacentTriangles;

            Math::BVH::Tree bvh;

            TriangleMesh(const std::vector<Math::Vector3D>& vertices, const std::vector<Math::Vector3S>& indices, const Math::Vector3D& defaultColor = {0.5, 0.5, 0.5});
            explicit TriangleMesh(const std::string& filename, const Math::Vector3D& defaultColor = {0.5, 0.5, 0.5});

            [[nodiscard]] std::vector<Math::Vector3D> transform() const;

            void updateAdjacentList();

            void updateVertexTangents();

            void updateVertexNormals();

            void updateTriangleAttributes();

            void updateBoundingInfo();

            void updateTransformedVertex();

            [[nodiscard]] Math::Vector3D getTransformedVertex(std::size_t idx) const;

            [[nodiscard]] Data::IntersectionInfo intersect(const Data::Ray &ray, double eps) override;
        };
    } // UltRenderer
} // Data

#endif //ULTRENDERER_TRIANGLEMESH_H
