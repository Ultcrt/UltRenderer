//
// Created by ultcrt on 23-11-23.
//

#include <unordered_set>
#include <fstream>
#include <exception>
#include <format>
#include <string>
#include <iostream>
#include <sstream>
#include "data/TriangleMesh.h"
#include "utils/String.h"
#include "utils/Hash.h"

namespace UltRenderer {
    namespace Data {
        TriangleMesh::TriangleMesh(const std::vector<Vector3D> &vertices, const std::vector<Vector3S> &indices, const Vector3D& defaultColor): _vertices(vertices), _indices(indices), _vertexColors(vertices.size(), defaultColor) {}

        TriangleMesh::TriangleMesh(const std::string &filename, const Vector3D& defaultColor) {
            std::ifstream loader(filename);
            if (!loader.is_open()) {
                throw std::runtime_error(std::format("Can not open file: {}", filename));
            }

            // Index in f starts at 1, so we can use index 0 as default value when index is not defined
            std::vector<Vector3D>     vList = {{0, 0, 0}};
            std::vector<Vector3D> colorList = {defaultColor};
            std::vector<Vector3D>    vtList = {{0, 0, 0}};
            std::vector<Vector3D>    vnList = {{0, 0, 0}};
            std::vector<Vector3D>    vpList = {{0, 0, 0}};

            std::vector<std::array<Vector3S, 3>> fList;

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
                            vertexParams.emplace_back(std::stod(param));
                        }

                        switch (vertexParams.size()) {
                            default:
                                throw std::runtime_error(std::format("Warning: Unexpected number of vertex params at line {}", lineIdx));
                                break;
                            case 3:
                                vList.emplace_back(vertexParams[0], vertexParams[1], vertexParams[2]);
                                break;
                            case 4:
                                if (vertexParams[3] == 0) {
                                    throw std::runtime_error(std::format("Warning: w param of vertex line is zero at line {}", lineIdx));
                                }
                                else {
                                    vList.emplace_back(Vector3D(vertexParams[0], vertexParams[1], vertexParams[2]) / vertexParams[3]);
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
                            textureParams.emplace_back(std::stod(param));
                        }

                        if (!textureParams.empty() && textureParams.size() < 4) {
                            Vector3D textureVec;
                            for (std::size_t idx = 0; idx < textureParams.size(); idx++) {
                                textureVec[idx] = textureParams[idx];
                            }
                            vtList.emplace_back(textureVec);
                        }
                        else {
                            throw std::runtime_error(std::format("Warning: Unexpected number of texture params at line {}", lineIdx));
                        }
                    }
                    else if (command == "vn") {
                        std::vector<double> normalParams;
                        for (const auto& param: params) {
                            normalParams.emplace_back(std::stod(param));
                        }

                        if (normalParams.size() == 3) {
                            _normals.emplace_back(normalParams[0], normalParams[1], normalParams[2]);
                        }
                        else {
                            throw std::runtime_error(std::format("Warning: Unexpected number of normal params at line {}", lineIdx));
                        }
                    }
                    else if (command == "vp") {
                        std::vector<double> vpParams;
                        for (const auto& param: params) {
                            vpParams.emplace_back(std::stod(param));
                        }

                        if (vpParams.size() == 3) {
                            Vector3D vpVec;
                            for (std::size_t idx = 0; idx < vpParams.size(); idx++) {
                                vpVec[idx] = vpParams[idx];
                            }
                            vpList.emplace_back(vpVec);
                        }
                        else {
                            throw std::runtime_error(std::format("Warning: Unexpected number of parameter space params at line {}", lineIdx));
                        }
                    }
                    else if (command == "f") {
                        // Split with /
                        if (params.size() == 3) {
                            std::array<Vector3S, 3> faceParams;
                            for (std::size_t paramIdx = 0; paramIdx < params.size(); paramIdx++) {
                                const auto& param = params[paramIdx];
                                std::vector<std::string> fIndices = Utils::String::Split(param, '/');

                                if (fIndices.size() <= 3) {
                                    Vector3S indices;
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
                                    throw std::runtime_error(std::format("Warning: Unexpected number of face params at line {}", lineIdx));
                                }
                            }
                            fList.emplace_back(faceParams);
                        }
                        else {
                            throw std::runtime_error(std::format("Warning: Unexpected number of face params at line {}", lineIdx));
                        }
                    }
                    else {
                        throw std::runtime_error(std::format("Warning: Unknown command '{}' at line {}", line[0], lineIdx));
                    }
                }
                lineIdx++;
            }

            const std::size_t bucketSize = 2 * fList.size();
            std::unordered_set<Vector3S, Utils::SpatialHash3D<std::size_t>> fSet(bucketSize, Utils::SpatialHash3D<std::size_t>(bucketSize));
            for (const auto& f: fList) {
                for (const auto& idxGroup: f) {
                    fSet.insert(idxGroup);
                }
            }
        }
    } // UltRenderer
} // Data