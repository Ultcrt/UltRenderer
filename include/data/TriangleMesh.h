//
// Created by ultcrt on 23-11-23.
//

#ifndef ULTRENDERER_TRIANGLEMESH_H
#define ULTRENDERER_TRIANGLEMESH_H

#include <vector>
#include <string>
#include "data/Matrix.h"

namespace UltRenderer {
    namespace Data {

        class TriangleMesh {
        public:
            std::vector<Vector3D> vertices;
            std::vector<Vector3D> vertexNormals;
            std::vector<Vector3S> triangles;
            std::vector<Vector3D> vertexColors;
            std::vector<Vector3D> vertexTextures;

            TriangleMesh(const std::vector<Vector3D>& vertices, const std::vector<Vector3S>& indices, const Vector3D& defaultColor = {0.5, 0.5, 0.5});
            explicit TriangleMesh(const std::string& filename, const Vector3D& defaultColor = {0.5, 0.5, 0.5});
        };

    } // UltRenderer
} // Data

#endif //ULTRENDERER_TRIANGLEMESH_H
