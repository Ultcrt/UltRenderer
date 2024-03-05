//
// Created by ultcrt on 24-3-4.
//

#ifndef ULTRENDERER_IAREALIGHT_H
#define ULTRENDERER_IAREALIGHT_H

#include "hierarchy/IntersectableNode.h"
#include "rendering/material/BSDFMaterial.h"

namespace UltRenderer {
    namespace Rendering {
        namespace Light {
            class IAreaLight: public Hierarchy::IntersectableNode {
            public:
                // Radiance
                double intensity = 1;

                explicit IAreaLight(double i);

                virtual Math::Vector3D sample(std::size_t n, const Math::Vector3D &p, const Material::CommonMaterial& target, const Math::Vector3D& v, const Math::Vector3D& uv, const Math::Vector3D& normal) const = 0;
            };
        } // Light
    } // Rendering
} // UltRenderer

#endif //ULTRENDERER_IAREALIGHT_H