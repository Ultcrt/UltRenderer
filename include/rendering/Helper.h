//
// Created by ultcrt on 24-1-17.
//

#ifndef ULTRENDERER_HELPER_H
#define ULTRENDERER_HELPER_H

#include "data/Image.h"
#include "data/TriangleMesh.h"
#include "math/Matrix.h"
#include "rendering/Camera.h"

namespace UltRenderer {
    namespace Rendering {
        Data::Image RenderDepthImageOfMesh(const Data::TriangleMesh& mesh, const Math::Vector3D& dir);
    } // Rendering
} // UltRenderer

#endif //ULTRENDERER_HELPER_H
