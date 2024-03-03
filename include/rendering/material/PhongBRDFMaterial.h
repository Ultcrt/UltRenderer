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
                Math::Vector3D evalBSDF(const Math::Vector3D &uv, const Math::Vector3D& normal, const Math::Vector3D& view, const Math::Vector3D& light) const override;
            };
        } // Material
    } // Rendering
} // UltRenderer

#endif //ULTRENDERER_PHONGBRDFMATERIAL_H
