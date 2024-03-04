//
// Created by ultcrt on 24-3-4.
//

#include "rendering/light/DirectionalLight.h"

namespace UltRenderer {
    namespace Rendering {
        namespace Light {
            Math::Vector3D DirectionalLight::getDirectionAt(const Math::Vector3D &point) const {
                return direction;
            }

            double DirectionalLight::getDistanceTo(const Math::Vector3D &point) const {
                // Return 1 to make everywhere have the same irradiance (intensity in this case)
                return 1.;
            }

            DirectionalLight::DirectionalLight(const Math::Vector3D &dir, double i): INonAreaLight(i), direction(dir) {}
        } // Light
    } // Rendering
} // UltRenderer