//
// Created by ultcrt on 24-3-4.
//

#include "rendering/light/IAreaLight.h"
#include "rendering/material/GlowBSDFMaterial.h"

namespace UltRenderer {
    namespace Rendering {
        namespace Light {
            IAreaLight::IAreaLight(const Math::Vector3D &i) {
                pMaterial = std::make_shared<Material::GlowBSDFMaterial>();
                intensity() = i;
            }

            Utils::MatrixProxy<double, 3, 1> IAreaLight::intensity() {
                return Utils::MatrixProxy<double, 3, 1>({&pMaterial->glowIntensity[0], &pMaterial->glowIntensity[1], &pMaterial->glowIntensity[2]});
            }

            Math::Vector3D IAreaLight::intensity() const {
                return pMaterial->glowIntensity;
            }
        } // Light
    } // Rendering
} // UltRenderer