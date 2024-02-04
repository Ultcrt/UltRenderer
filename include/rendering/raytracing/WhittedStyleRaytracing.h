//
// Created by ultcrt on 24-1-22.
//

#ifndef ULTRENDERER_WHITTEDSTYLERAYTRACING_H
#define ULTRENDERER_WHITTEDSTYLERAYTRACING_H

#include <memory>
#include <vector>
#include "math/Matrix.h"
#include "math/Ray.h"
#include "data/TriangleMesh.h"

namespace UltRenderer {
    namespace Rendering {
        namespace Raytracing {
            Data::Color<Data::ColorFormat::RGBA> Cast(
                    const Math::Ray& ray,
                    const Scene* pScene,
                    const Math::Vector4D& backgroundColor = {1, 1, 1, 1},
                    double eps = 1e-6,
                    std::size_t maxRecursion = 3);
        } // Raytracing
    } // Rendering
} // UltRenderer

#endif //ULTRENDERER_WHITTEDSTYLERAYTRACING_H
