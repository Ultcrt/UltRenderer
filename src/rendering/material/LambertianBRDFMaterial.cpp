//
// Created by ultcrt on 24-3-2.
//

#include "rendering/material/LambertianBRDFMaterial.h"
#include "rendering/BSDF.h"

namespace UltRenderer {
    namespace Rendering {
        namespace Material {
            Math::Vector3D LambertianBRDFMaterial::getBSDF(const Math::Vector3D &uv) const {
                return BSDF::LambertianDiffuseBRDF(uv, *this);
            }
        } // Material
    } // Rendering
} // UltRenderer