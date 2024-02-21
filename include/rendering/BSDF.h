//
// Created by ultcrt on 24-2-21.
//

#ifndef ULTRENDERER_BSDF_H
#define ULTRENDERER_BSDF_H

#include "math/Matrix.h"
#include "rendering/Material.h"

namespace UltRenderer {
    namespace Rendering {
        namespace BSDF {
            Math::Vector3D LambertianDiffuseBRDF(const Math::Vector3D& uv, const Material& mat);
        } // BSDF
    } // Rendering
} // UltRenderer

#endif //ULTRENDERER_BSDF_H
