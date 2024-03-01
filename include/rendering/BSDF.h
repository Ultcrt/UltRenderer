//
// Created by ultcrt on 24-2-21.
//

#ifndef ULTRENDERER_BSDF_H
#define ULTRENDERER_BSDF_H

#include "math/Matrix.h"
#include "rendering/material/CommonMaterial.h"

namespace UltRenderer {
    namespace Rendering {
        namespace BSDF {
            Math::Vector3D LambertianDiffuseBRDF(const Math::Vector3D& uv, const Material::CommonMaterial& mat);
        } // BSDF
    } // Rendering
} // UltRenderer

#endif //ULTRENDERER_BSDF_H
