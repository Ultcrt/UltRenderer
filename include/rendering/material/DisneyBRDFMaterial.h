//
// Created by ultcrt on 24-3-3.
//

#ifndef ULTRENDERER_DISNEYBRDFMATERIAL_H
#define ULTRENDERER_DISNEYBRDFMATERIAL_H

#include "BSDFMaterial.h"

namespace UltRenderer {
    namespace Rendering {
        namespace Material {
            class DisneyBRDFMaterial: public BSDFMaterial {
            public:
                double roughness = 0.1;
                [[nodiscard]] Math::Vector3D evalBSDF(const Math::Vector3D &uv, const Math::Vector3D &normal, const Math::Vector3D &view, const Math::Vector3D &light) const override;
            };
        } // Material
    } // Rendering
} // UltRenderer

#endif //ULTRENDERER_DISNEYBRDFMATERIAL_H
