//
// Created by ultcrt on 24-2-21.
//

#include "rendering/BSDF.h"

namespace UltRenderer {
    namespace Rendering {
        namespace BSDF {
            Math::Vector3D LambertianDiffuseBRDF(const Math::Vector3D& uv, const Material::CommonMaterial& mat) {
                Math::Vector3D color = mat.getDiffuseColor(uv);

                return color * mat.diffuseCoefficient / M_PI;
            }

            Math::Vector3D PhongReflectionBRDF(const Math::Vector3D& uv, const Material::CommonMaterial& mat) {

            }
        } // BSDF
    } // Rendering
} // UltRenderer