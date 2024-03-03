//
// Created by ultcrt on 24-3-3.
//

#include "rendering/material/DisneyBRDFMaterial.h"

namespace UltRenderer {
    namespace Rendering {
        namespace Material {
            Math::Vector3D DisneyBRDFMaterial::evalBSDF(const Math::Vector3D &uv, const Math::Vector3D &normal,
                                                        const Math::Vector3D &view, const Math::Vector3D &light) const {
                Math::Vector3D half = (view - light) / 2;
                double cosThetaV = view.dot(normal);
                double cosThetaL = -light.dot(normal);
                double fD90 = 0.5 + 2 * roughness * std::pow(half.dot(-light), 2.);
                return getDiffuseColor(uv) / M_PI * (1 + (fD90 - 1) * std::pow(1. - cosThetaL, 5.)) * (1 + (fD90 - 1) * std::pow(1. - cosThetaV, 5.));
            }
        } // Material
    } // Rendering
} // UltRenderer