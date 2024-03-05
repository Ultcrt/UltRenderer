//
// Created by ultcrt on 24-3-5.
//

#include "rendering/material/GlowBSDFMaterial.h"

namespace UltRenderer {
    namespace Rendering {
        namespace Material {
            Math::Vector3D GlowBSDFMaterial::evalBSDF(const Math::Vector3D &uv, const Math::Vector3D &normal,
                                                      const Math::Vector3D &view, const Math::Vector3D &light) const {
                return glowIntensity * Math::Vector3D(1, 1, 1);
            }
        } // Material
    } // Rendering
} // UltRenderer