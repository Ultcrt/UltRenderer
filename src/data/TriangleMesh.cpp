//
// Created by ultcrt on 23-11-23.
//

#include <set>
#include <fstream>
#include <exception>
#include <format>
#include <string>
#include <iostream>
#include <sstream>
#include "data/TriangleMesh.h"
#include "utils/String.h"

namespace UltRenderer {
    namespace Data {
        TriangleMesh::TriangleMesh(const std::vector<Vector3D> &vertices, const std::vector<Vector3S> &indices, const Vector3D& defaultColor): _vertices(vertices), _indices(indices), _vertexColors(vertices.size(), defaultColor) {}

        TriangleMesh::TriangleMesh(const std::string &filename, const Vector3D& defaultColor) {
            std::ifstream loader(filename);
            if (!loader.is_open()) {
                throw std::runtime_error(std::format("Can not open file: {}", filename));
            }

            // Index in f starts at 1, so we can use index 0 as default value when index is not defined
            std::vector<Vector3D>  vList = {{0, 0, 0}};
            std::vector<Vector3D> vtList = {{0, 0, 0}};
            std::vector<Vector3D> vnList = {{0, 0, 0}};
            std::vector<Vector3D> vpList = {{0, 0, 0}};

            std::vector<std::array<Vector3S, 3>> fList;
            std::set<std::string> fSet;

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
                                std::cerr << std::format("Warning: Unexpected number of vertex params at line {}", lineIdx);
                                break;
                            case 3:
                                _vertices.emplace_back(vertexParams[0], vertexParams[1], vertexParams[2]);
                                _vertexColors.emplace_back(defaultColor);
                                break;
                            case 4:
                                if (vertexParams[3] == 0) {
                                    std::cerr << std::format("Warning: w param of vertex line is zero at line {}", lineIdx);
                                }
                                else {
                                    _vertices.emplace_back(Vector3D(vertexParams[0], vertexParams[1], vertexParams[2]) / vertexParams[3]);
                                    _vertexColors.emplace_back(defaultColor);
                                }
                                break;
                            case 6:
                                _vertices.emplace_back(vertexParams[0], vertexParams[1], vertexParams[2]);
                                _vertexColors.emplace_back(vertexParams[1], vertexParams[2], vertexParams[3]);
                                break;
                        }
                    }
                    else if (command == "vt") {
                        // TODO Implement texture parser
                        continue;
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
                            std::cerr << std::format("Warning: Unexpected number of normal params at line {}", lineIdx);
                        }
                    }
                    else if (command == "vp") {
                        // TODO Implement parameter parser
                        continue;
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
                                    fSet.insert(std::string(indices));
                                }
                                else {
                                    std::cerr << std::format("Warning: Unexpected number of face params at line {}", lineIdx);
                                }
                            }
                        }
                        else {
                            std::cerr << std::format("Warning: Unexpected number of face params at line {}", lineIdx);
                        }
                    }
                    else {
                        std::cerr << std::format("Warning: Unknown command '{}' at line {}", line[0], lineIdx);
                    }
                }
                lineIdx++;
            }


        }
    } // UltRenderer
} // Data