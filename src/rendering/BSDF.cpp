//
// Created by ultcrt on 24-2-21.
//

#include "rendering/BSDF.h"

namespace UltRenderer {
    namespace Rendering {
        namespace BSDF {
            Math::Vector3D LambertianDiffuseBRDF(const Math::Vector3D& uv, const Material::CommonMaterial& mat) {
                Math::Vector3D color;
                if (mat.pTexture->type() == Data::ColorFormat::GRAY) {
                    color = mat.pTexture->get<Data::ColorFormat::GRAY>(uv[0], uv[1]).to<Data::ColorFormat::RGB>();
                }
                else {
                    color = mat.pTexture->get<Data::ColorFormat::RGB>(uv[0], uv[1]);
                }

                return color * mat.diffuseCoefficient / M_PI;
            }

            Math::Vector3D PhongReflectionBRDF(const Math::Vector3D& uv, const Material::CommonMaterial& mat) {

            }
        } // BSDF
    } // Rendering
} // UltRenderer