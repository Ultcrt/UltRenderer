//
// Created by ultcrt on 24-3-3.
//

#include <iostream>
#include "rendering/material/PhongBRDFMaterial.h"
#include "math/Geometry.h"

namespace UltRenderer {
    namespace Rendering {
        namespace Material {
            Math::Vector3D PhongBRDFMaterial::evalBSDF(const Math::Vector3D &uv, const Math::Vector3D& normal, const Math::Vector3D& view, const Math::Vector3D& light) const {
                const Math::Vector3D& reflect = Math::Geometry::ComputeReflectionDirection(normal, light);

                auto [brightness, finalSpecularColor] = getSpecularInfo(uv);

                return finalSpecularColor * std::pow(std::max(reflect.dot(view), 0.), brightness);
            }
        } // Material
    } // Rendering
} // UltRenderer