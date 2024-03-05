//
// Created by ultcrt on 24-3-5.
//

#ifndef ULTRENDERER_GLOWBSDFMATERIAL_H
#define ULTRENDERER_GLOWBSDFMATERIAL_H

#include "BSDFMaterial.h"

namespace UltRenderer {
    namespace Rendering {
        namespace Material {
            class GlowBSDFMaterial: public BSDFMaterial {
            public:
                [[nodiscard]] Math::Vector3D evalBSDF(const Math::Vector3D &uv, const Math::Vector3D &normal, const Math::Vector3D &view, const Math::Vector3D &light) const override;
            };
        } // Material
    } // Rendering
} // UltRenderer

#endif //ULTRENDERER_GLOWBSDFMATERIAL_H
