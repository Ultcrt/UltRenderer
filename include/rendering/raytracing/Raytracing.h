//
// Created by ultcrt on 24-1-22.
//

#ifndef ULTRENDERER_RAYTRACING_H
#define ULTRENDERER_RAYTRACING_H

#include <memory>
#include <vector>
#include "math/Matrix.h"
#include "math/Ray.h"
#include "data/TriangleMesh.h"

namespace UltRenderer {
    namespace Rendering {
        namespace Raytracing {
            Math::Vector4D Cast(
                    const Math::Ray& ray,
                    const std::vector<std::shared_ptr<Data::TriangleMesh>>& pMeshes,
                    const Math::Vector4D& backgroundColor = {1, 1, 1, 1},
                    double eps = 1e-6,
                    std::size_t maxRecursion = 5);
        } // Raytracing
    } // Rendering
} // UltRenderer

#endif //ULTRENDERER_RAYTRACING_H
