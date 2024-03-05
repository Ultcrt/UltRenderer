//
// Created by ultcrt on 24-3-4.
//

#ifndef ULTRENDERER_BSDFMATERIAL_H
#define ULTRENDERER_BSDFMATERIAL_H

#include "CommonMaterial.h"

namespace UltRenderer {
    namespace Rendering {
        namespace Material {
            class BSDFMaterial: public CommonMaterial {
            public:
                /**
                 * Uniform sampling
                 * @param uv The UV of shading point
                 * @param n The number of samples
                 * @param normal The normal of shading point
                 * @param view The view direction of shading point (point to eye)
                 * @return
                 */
                [[nodiscard]] std::pair<std::vector<Math::Vector3D>, std::vector<Math::Vector3D>> sampleBSDF(const Math::Vector3D &uv, std::size_t n, const Math::Vector3D &normal, const Math::Vector3D &view) const override;
            };
        } // Material
    } // Rendering
} // UltRenderer

#endif //ULTRENDERER_BSDFMATERIAL_H
