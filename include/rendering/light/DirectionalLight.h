//
// Created by ultcrt on 24-3-4.
//

#ifndef ULTRENDERER_DIRECTIONALLIGHT_H
#define ULTRENDERER_DIRECTIONALLIGHT_H

#include "INonAreaLight.h"

namespace UltRenderer {
    namespace Rendering {
        namespace Light {
            class DirectionalLight: public INonAreaLight {
            public:
                Math::Vector3D direction = {0, -1, 0};

                explicit DirectionalLight(const Math::Vector3D& dir, double i = 1);

                [[nodiscard]] Math::Vector3D getDirectionAt(const Math::Vector3D &point) const override;
                [[nodiscard]] double getDistanceTo(const Math::Vector3D &point) const override;
            };
        } // Light
    } // Rendering
} // UltRenderer

#endif //ULTRENDERER_DIRECTIONALLIGHT_H
