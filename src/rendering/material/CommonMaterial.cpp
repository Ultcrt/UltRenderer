//
// Created by ultcrt on 24-1-30.
//

#include "rendering/material/CommonMaterial.h"

namespace UltRenderer {
    namespace Rendering {
        Math::Vector3D Material::CommonMaterial::getBSDF(const Math::Vector3D& uv, const Math::Vector3D& normal, const Math::Vector3D& view, const Math::Vector3D& light) const {
            throw std::runtime_error("Material not support Physic based rendering (i.e. not implement getBSDF method)");
        }
    } // Rendering
} // UltRenderer