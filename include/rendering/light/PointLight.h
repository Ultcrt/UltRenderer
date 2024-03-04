//
// Created by ultcrt on 24-3-4.
//

#ifndef ULTRENDERER_POINTLIGHT_H
#define ULTRENDERER_POINTLIGHT_H

#include "INonAreaLight.h"

namespace UltRenderer {
    namespace Rendering {
        namespace Light {
            class PointLight: public INonAreaLight {
            public:
                Math::Vector3D position;

                explicit PointLight(const Math::Vector3D& p, double i = 1);

                [[nodiscard]] Math::Vector3D getDirectionAt(const Math::Vector3D &point) const override;
                [[nodiscard]] double getDistanceTo(const Math::Vector3D &point) const override;
            };
        } // Light
    } // Rendering
} // UltRenderer

#endif //ULTRENDERER_POINTLIGHT_H
