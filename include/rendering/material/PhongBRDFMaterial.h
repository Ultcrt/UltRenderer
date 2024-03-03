//
// Created by ultcrt on 24-3-3.
//

#ifndef ULTRENDERER_PHONGBRDFMATERIAL_H
#define ULTRENDERER_PHONGBRDFMATERIAL_H

#include "CommonMaterial.h"

namespace UltRenderer {
    namespace Rendering {
        namespace Material {
            class PhongBRDFMaterial: public CommonMaterial {
            public:
                [[nodiscard]] Math::Vector3D evalBSDF(const Math::Vector3D &uv, const Math::Vector3D& normal, const Math::Vector3D& view, const Math::Vector3D& light) const override;
                [[nodiscard]] std::pair<std::vector<Math::Vector3D>, std::vector<Math::Vector3D>> sampleBSDF(const Math::Vector3D &uv, std::size_t n, const Math::Vector3D &normal, const Math::Vector3D &view) const override;
            };
        } // Material
    } // Rendering
} // UltRenderer

#endif //ULTRENDERER_PHONGBRDFMATERIAL_H
