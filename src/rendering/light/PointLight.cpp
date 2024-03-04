//
// Created by ultcrt on 24-3-4.
//

#include "rendering/light/PointLight.h"

namespace UltRenderer {
    namespace Rendering {
        namespace Light {
            Math::Vector3D PointLight::getDirectionAt(const Math::Vector3D &point) const {
                return (point - position).normalized();
            }

            double PointLight::getDistanceTo(const Math::Vector3D &point) const {
                return (point - position).norm();
            }

            PointLight::PointLight(const Math::Vector3D &p, double i): INonAreaLight(i), position(p) {}
        } // Light
    } // Rendering
} // UltRenderer