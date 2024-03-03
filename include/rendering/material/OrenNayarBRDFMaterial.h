//
// Created by ultcrt on 24-3-3.
//

#ifndef ULTRENDERER_ORENNAYARBRDFMATERIAL_H
#define ULTRENDERER_ORENNAYARBRDFMATERIAL_H

#include "CommonMaterial.h"

namespace UltRenderer {
    namespace Rendering {
        namespace Material {
            class OrenNayarBRDFMaterial: public CommonMaterial {
            public:
                double roughness = 0.1;

                Math::Vector3D getBSDF(const Math::Vector3D &uv, const Math::Vector3D &normal, const Math::Vector3D &view, const Math::Vector3D &light) const override;
            };
        } // Material
    } // Rendering
} // UltRenderer

#endif //ULTRENDERER_ORENNAYARBRDFMATERIAL_H
