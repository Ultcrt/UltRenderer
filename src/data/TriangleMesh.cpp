//
// Created by ultcrt on 23-11-23.
//

#include <unordered_map>
#include <fstream>
#include <exception>
#include <format>
#include <string>
#include <iostream>
#include <sstream>
#include "data/TriangleMesh.h"
#include "utils/String.h"
#include "utils/Hash.h"
#include "math/Geometry.h"

namespace UltRenderer {
    namespace Data {
        TriangleMesh::TriangleMesh(const std::vector<Math::Vector3D> &vertices, const std::vector<Math::Vector3S> &indices, const Math::Vector3D& defaultColor): vertices(vertices), triangles(indices), vertexColors(vertices.size(), defaultColor), bvh(vertices, indices) {
            updateTransformedVertex();
            updateTriangleAttributes();
            updateAdjacentList();
            updateVertexNormals();
            updateVertexTangents();
            updateBoundingInfo();
        }

        TriangleMesh::TriangleMesh(const std::string &filename, const Math::Vector3D& defaultColor) {
            std::ifstream loader(filename);
            if (!loader.is_open()) {
                throw std::runtime_error(std::format("Can not open file: {}", filename));
            }

            // Index in f starts at 1, so we can use index 0 as default value when index is not defined
            std::vector<Math::Vector3D>     vList = {{0, 0, 0}};
            std::vector<Math::Vector3D> colorList = {defaultColor};
            std::vector<Math::Vector3D>    vtList = {{0, 0, 0}};
            std::vector<Math::Vector3D>    vnList = {{0, 0, 0}};
            std::vector<Math::Vector3D>    vpList = {{0, 0, 0}};

            std::vector<std::array<Math::Vector3S, 3>> fList;

            std::string line;
            std::size_t lineIdx = 0;
            while (std::getline(loader, line)) {
                // Split with space
                std::vector<std::string> split = Utils::String::Split(line);

                if (split.size() > 1) {
                    std::string command = split[0];
                    std::vector<std::string> params(split.begin() + 1, split.end());

                    // Process .obj file based on first letter
                    if (command == "#") {
                        continue;
                    }
                    else if (command == "v") {
                        std::vector<double> vertexParams;
                        for (const auto& param: params) {
                            if (!param.empty()) {
                                vertexParams.emplace_back(std::stod(param));
                            }
                        }

                        switch (vertexParams.size()) {
                            default:
                                throw std::runtime_error(std::format("Unexpected number of vertex params at line {}", lineIdx));
                            case 3:
                                vList.emplace_back(vertexParams[0], vertexParams[1], vertexParams[2]);
                                break;
                            case 4:
                                if (vertexParams[3] == 0) {
                                    throw std::runtime_error(std::format("w param of vertex line is zero at line {}", lineIdx));
                                }
                                else {
                                    vList.emplace_back(Math::Vector3D(vertexParams[0], vertexParams[1], vertexParams[2]) / vertexParams[3]);
                                }
                                break;
                            case 6:
                                vList.emplace_back(vertexParams[0], vertexParams[1], vertexParams[2]);
                                colorList.emplace_back(vertexParams[1], vertexParams[2], vertexParams[3]);
                                break;
                        }
                    }
                    else if (command == "vt") {
                        std::vector<double> textureParams;
                        for (const auto& param: params) {
                            if (!param.empty()) {
                                textureParams.emplace_back(std::stod(param));
                            }
                        }

                        if (!textureParams.empty() && textureParams.size() < 4) {
                            Math::Vector3D textureVec;
                            for (std::size_t idx = 0; idx < textureParams.size(); idx++) {
                                textureVec[idx] = textureParams[idx];
                            }
                            vtList.emplace_back(textureVec);
                        }
                        else {
                            throw std::runtime_error(std::format("Unexpected number of texture params at line {}", lineIdx));
                        }
                    }
                    else if (command == "vn") {
                        std::vector<double> normalParams;
                        for (const auto& param: params) {
                            if (!param.empty()) {
                                normalParams.emplace_back(std::stod(param));
                            }
                        }

                        if (normalParams.size() == 3) {
                            vnList.emplace_back(Math::Vector3D{normalParams[0], normalParams[1], normalParams[2]}.normalized());
                        }
                        else {
                            throw std::runtime_error(std::format("Unexpected number of normal params at line {}", lineIdx));
                        }
                    }
                    else if (command == "vp") {
                        std::vector<double> vpParams;
                        for (const auto& param: params) {
                            if (!param.empty()) {
                                vpParams.emplace_back(std::stod(param));
                            }
                        }

                        if (vpParams.size() == 3) {
                            Math::Vector3D vpVec;
                            for (std::size_t idx = 0; idx < vpParams.size(); idx++) {
                                vpVec[idx] = vpParams[idx];
                            }
                            vpList.emplace_back(vpVec);
                        }
                        else {
                            throw std::runtime_error(std::format("Unexpected number of parameter space params at line {}", lineIdx));
                        }
                    }
                    else if (command == "f") {
                        // Split with /
                        if (params.size() == 3) {
                            std::array<Math::Vector3S, 3> faceParams;
                            for (std::size_t paramIdx = 0; paramIdx < params.size(); paramIdx++) {
                                const auto& param = params[paramIdx];
                                std::vector<std::string> fIndices = Utils::String::Split(param, '/');

                                if (fIndices.size() <= 3) {
                                    Math::Vector3S indices;
                                    for (std::size_t idx = 0; idx < fIndices.size(); idx++) {
                                        if (fIndices[idx].empty()) {
                                            // Set to default idx when index is ignored
                                            indices[idx] = 0;
                                        }
                                        else {
                                            indices[idx] = std::stoull(fIndices[idx]);
                                        }
                                    }
                                    faceParams[paramIdx] = indices;
                                }
                                else {
                                    throw std::runtime_error(std::format("Unexpected number of face params at line {}", lineIdx));
                                }
                            }
                            fList.emplace_back(faceParams);
                        }
                        else {
                            throw std::runtime_error(std::format("Unexpected number of face params at line {}", lineIdx));
                        }
                    }
                    else if (command == "o") {
                        // TODO: Implement object parse
                        continue;
                    }
                    else if (command == "g") {
                        // TODO: Implement group parse
                        continue;
                    }
                    else if (command == "s") {
                        // TODO: Implement group smoothing
                        continue;
                    }
                    else {
                        throw std::runtime_error(std::format("Unknown command '{}' at line {}", line[0], lineIdx));
                    }
                }
                lineIdx++;
            }

            // Create f map
            // TODO: Need dealing with no vt obj
            const std::size_t bucketSize = 2 * fList.size();
            std::unordered_map<Math::Vector3S, std::size_t, Utils::Hash::SpatialHash3D<std::size_t>> fMap(bucketSize, Utils::Hash::SpatialHash3D<std::size_t>(bucketSize));
            for (const auto& f: fList) {
                for (const auto& idxGroup: f) {
                    // Tips: map::insert will not update already-exist key-value pair
                    fMap.insert({idxGroup, fMap.size()});
                }
            }

            // Convert to OpenGL-like vertex
            vertices.resize(fMap.size());
            vertexColors.resize(fMap.size());
            vertexTextures.resize(fMap.size());
            vertexNormals.resize(fMap.size());
            for (const auto& item: fMap) {
                const auto & key = item.first;
                const auto & val = item.second;
                vertices[val] = vList[key[0]];
                vertexColors[val] = colorList[key[0]];
                vertexTextures[val] = vtList[key[1]];
                vertexNormals[val] = vnList[key[2]];
            }

            // Create triangles
            for (const auto& f: fList) {
                triangles.emplace_back(fMap[f[0]], fMap[f[1]], fMap[f[2]]);
            }

            // Update computed attributes
            updateTransformedVertex();
            updateTriangleAttributes();
            updateAdjacentList();
            updateVertexTangents();
            updateBoundingInfo();
            if (vnList.empty()) {
                updateVertexNormals();
            }

            bvh = Math::BVH::Tree(_transformedVertices, triangles);
        }

        std::vector<Math::Vector3D> TriangleMesh::transform() const {
            return _transformedVertices;
        }

        void TriangleMesh::updateVertexTangents() {
            vertexTangents = {};
            for (std::size_t idx = 0; idx < vertices.size(); idx++) {
                const auto adjacency = adjacentTriangles[idx];

                Math::Vector3D tangent = {0., 0., 0.};
                for (const auto tIdx: adjacency) {
                    const auto v0 = vertices[triangles[tIdx][0]];
                    const auto v1 = vertices[triangles[tIdx][1]];
                    const auto v2 = vertices[triangles[tIdx][2]];

                    const double area = (v1 - v0).cross(v2 - v0).norm();

                    tangent += area * triangleTangents[tIdx];
                }

                vertexTangents.emplace_back(tangent.normalized());
            }
        }

        void TriangleMesh::updateVertexNormals() {
            vertexNormals = {};
            for (std::size_t idx = 0; idx < vertices.size(); idx++) {
                const auto adjacency = adjacentTriangles[idx];

                Math::Vector3D normal = {0., 0., 0.};
                for (const auto tIdx: adjacency) {
                    const auto v0 = vertices[triangles[tIdx][0]];
                    const auto v1 = vertices[triangles[tIdx][1]];
                    const auto v2 = vertices[triangles[tIdx][2]];

                    const double area = (v1 - v0).cross(v2 - v0).norm();

                    normal += area * triangleNormals[tIdx];
                }

                vertexNormals.emplace_back(normal.normalized());
            }
        }

        void TriangleMesh::updateTriangleAttributes() {
            triangleNormals = {};
            triangleTangents = {};

            if (vertexTextures.empty()) {
                // If uvs is empty, then use the first edge as tangent
                for (const auto triangle: triangles) {
                    const auto vertex0 = vertices[triangle[0]];
                    const auto vertex1 = vertices[triangle[1]];
                    const auto vertex2 = vertices[triangle[2]];

                    const auto vec01 = vertex1 - vertex0;
                    const auto vec02 = vertex2 - vertex0;

                    const auto normal = vec01.cross(vec02).normalized();
                    const auto tangent = vec01.normalized();

                    triangleNormals.emplace_back(normal);
                    triangleTangents.emplace_back(tangent);
                }
            }
            else {
                for (const auto triangle: triangles) {
                    const auto vertex0 = vertices[triangle[0]];
                    const auto vertex1 = vertices[triangle[1]];
                    const auto vertex2 = vertices[triangle[2]];
                    const auto u0 = vertexTextures[triangle[0]].x();
                    const auto u1 = vertexTextures[triangle[1]].x();
                    const auto u2 = vertexTextures[triangle[2]].x();

                    const auto vec01 = vertex1 - vertex0;
                    const auto vec02 = vertex2 - vertex0;

                    const auto normal = vec01.cross(vec02).normalized();

                    const Math::Matrix3D transform = {
                            vec01.x(), vec01.y(), vec01.z(),
                            vec02.x(), vec02.y(), vec02.z(),
                            normal.x(), normal.y(), normal.z()
                    };

                    const auto tangent = (transform.inverse() * Math::Vector3D{u1 - u0, u2 - u0, 0}).normalized();

                    triangleNormals.emplace_back(normal);
                    triangleTangents.emplace_back(tangent);
                }
            }
        }

        void TriangleMesh::updateAdjacentList() {
            adjacentVertices = std::vector<std::set<std::size_t>>{vertices.size()};
            adjacentTriangles = std::vector<std::set<std::size_t>>{vertices.size()};
            for (std::size_t tIdx = 0; tIdx < triangles.size(); tIdx++) {
                const auto triangle = triangles[tIdx];
                for (std::size_t idx = 0; idx < 3; idx++) {
                    adjacentVertices[triangle[idx]].insert(triangle[(idx + 1) % 3]);
                    adjacentVertices[triangle[idx]].insert(triangle[(idx + 2) % 3]);
                    adjacentTriangles[triangle[idx]].insert(tIdx);
                }
            }
        }

        void TriangleMesh::updateBoundingInfo() {
            boundingInfo = Math::Geometry::GetAABB(_transformedVertices);
        }

        Math::Vector3D TriangleMesh::getTransformedVertex(std::size_t idx) const {
            return _transformedVertices[idx];
        }

        void TriangleMesh::updateTransformedVertex() {
            _transformedVertices = {};

            for (const auto& vertex: vertices) {
                _transformedVertices.emplace_back((transformMatrix * vertex.toHomogeneousCoordinates(1)).toCartesianCoordinates());
            }
        }

        Data::IntersectionInfo TriangleMesh::intersect(const Data::Ray &ray, double eps) {
            bool stop = false;
            auto info = _intersectBVH(ray, bvh.pRoot.get(), stop, false, eps);
            return info;
        }

        Data::IntersectionInfo
        TriangleMesh::_intersectBVH(const Data::Ray& ray, const Math::BVH::Node *pNode, bool& stop, bool fastCheck, double eps) {
            Data::IntersectionInfo res;

            // Fast stop
            if (!stop) {
                // Check children when intersected with bounding box
                if (ray.isIntersected(pNode->boundingInfo)) {
                    // Leaf node, check primitives inside
                    if (pNode->pLeft == nullptr and pNode->pRight == nullptr) {
                        for (const auto& tIdx: pNode->primitiveIndices) {
                            const auto info = _intersectTriangle(ray, tIdx, eps);

                            if (info.isIntersected) {
                                if (info.length < res.length) {
                                    res = info;
                                    if (fastCheck) {
                                        stop = true;
                                    }
                                }
                            }
                        }
                    }
                    // Non-leaf node, check children nodes
                    else {
                        for (const auto& pChild: {pNode->pLeft.get(), pNode->pRight.get()}) {
                            if (pChild != nullptr) {
                                const auto newInfo = _intersectBVH(ray, pChild, stop, fastCheck, eps);

                                if (newInfo.isIntersected) {
                                    if (newInfo.length < res.length) {
                                        res = newInfo;
                                        if (fastCheck) {
                                            stop = true;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }

            return res;
        }

        Data::IntersectionInfo TriangleMesh::_intersectTriangle(const Data::Ray& ray, std::size_t triangleIdx, double eps) {
            const auto triangle = triangles[triangleIdx];

            // To world frame
            const auto p0 = getTransformedVertex(triangle[0]);
            const auto p1 = getTransformedVertex(triangle[1]);
            const auto p2 = getTransformedVertex(triangle[2]);
            auto uv0 = Math::Vector3D(0, 0, 0);
            auto uv1 = Math::Vector3D(0, 0, 0);
            auto uv2 = Math::Vector3D(0, 0, 0);

            if (!vertexTextures.empty()) {
                uv0 = vertexTextures[triangle[0]];
                uv1 = vertexTextures[triangle[1]];
                uv2 = vertexTextures[triangle[2]];
            }

            auto res = Math::Geometry::MollerTrumboreIntersection(ray, p0, p1, p2, eps);
            auto t = res.first;
            auto b0 = res.second[0];
            auto b1 = res.second[1];
            auto b2 = res.second[2];

            IntersectionInfo info;
            if (t >= 0 && b0 >= 0 && b1 >= 0 && b2 >= 0) {
                info.pNode = this;
                info.isIntersected = true;
                info.length = t;
                info.uv = uv0 * b0 + uv1 * b1 + uv2 * b2;

                auto normal = vertexNormals[0] * b0 + vertexNormals[1] * b1 + vertexNormals[2] * b2;
                if (pMaterial->pNormalMap) {
                    normal = (*pMaterial->pNormalMap).get<Data::ColorFormat::RGB>(info.uv[0], info.uv[1]) * 2. - Math::Vector3D{1, 1, 1};
                    if (pMaterial->normalMapType == Data::NormalMapType::DARBOUX) {
                        const auto& triangleNormal = (transformMatrix * triangleNormals[triangleIdx].toHomogeneousCoordinates(0)).toCartesianCoordinates();
                        const auto& triangleTangent = (transformMatrix * triangleTangents[triangleIdx].toHomogeneousCoordinates(0)).toCartesianCoordinates();

                        normal = Math::Geometry::ConvertDarbouxNormalToGlobal(triangleTangent, triangleNormal, normal);
                    }
                    else {
                        normal = (transformMatrix * normal.toHomogeneousCoordinates(0)).toCartesianCoordinates().normalized();
                    }
                }

                info.normal = normal;
            }
            return info;
        }
    } // UltRenderer
} // Data