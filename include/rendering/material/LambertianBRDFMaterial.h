//
// Created by ultcrt on 24-3-2.
//

#ifndef ULTRENDERER_LAMBERTIANBRDFMATERIAL_H
#define ULTRENDERER_LAMBERTIANBRDFMATERIAL_H

#include "CommonMaterial.h"

namespace UltRenderer {
    namespace Rendering {
        namespace Material {
            class LambertianBRDFMaterial: public CommonMaterial {
            public:
                Math::Vector3D evalBSDF(const Math::Vector3D& uv, const Math::Vector3D& normal, const Math::Vector3D& view, const Math::Vector3D& light) const override;
            };
        } // Material
    } // Rendering
} // UltRenderer

#endif //ULTRENDERER_LAMBERTIANBRDFMATERIAL_H
