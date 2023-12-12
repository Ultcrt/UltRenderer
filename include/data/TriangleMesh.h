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
        private:
            std::vector<Vector3D> _vertices;
            std::vector<Vector3D> _vertexNormals;
            std::vector<Vector3S> _triangles;
            std::vector<Vector3D> _vertexColors;
            std::vector<Vector3D> _vertexTextures;

        public:
            TriangleMesh(const std::vector<Vector3D>& vertices, const std::vector<Vector3S>& indices, const Vector3D& defaultColor = {0.5, 0.5, 0.5});
            explicit TriangleMesh(const std::string& filename, const Vector3D& defaultColor = {0.5, 0.5, 0.5});
        };

    } // UltRenderer
} // Data

#endif //ULTRENDERER_TRIANGLEMESH_H
