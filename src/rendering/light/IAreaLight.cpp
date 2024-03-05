//
// Created by ultcrt on 24-3-4.
//

#include "rendering/light/IAreaLight.h"
#include "rendering/material/GlowBSDFMaterial.h"

namespace UltRenderer {
    namespace Rendering {
        namespace Light {
            IAreaLight::IAreaLight(double i) {
                pMaterial = std::make_shared<Material::GlowBSDFMaterial>();
                intensity() = i;
            }

            double &IAreaLight::intensity() {
                return pMaterial->glowIntensity;
            }

            double IAreaLight::intensity() const {
                return pMaterial->glowIntensity;
            }
        } // Light
    } // Rendering
} // UltRenderer