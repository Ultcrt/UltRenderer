//
// Created by ultcrt on 24-1-17.
//

#ifndef ULTRENDERER_HELPER_H
#define ULTRENDERER_HELPER_H

#include "data/Image.h"
#include "data/TriangleMesh.h"
#include "math/Matrix.h"

namespace UltRenderer {
    namespace Rendering {
        void RenderDepthImageOfMesh(const Data::TriangleMesh& mesh, const Math::Vector3D& dir, Data::Image& depthImage,
                                                 Math::Transform3D* outModelView = nullptr,
                                                 Math::Transform3D* outProjection = nullptr,
                                                 Math::Transform3D* outViewport = nullptr);
    } // Rendering
} // UltRenderer

#endif //ULTRENDERER_HELPER_H
