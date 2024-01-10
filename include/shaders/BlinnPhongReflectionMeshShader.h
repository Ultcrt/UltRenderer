//
// Created by ultcrt on 24-1-10.
//

#ifndef ULTRENDERER_BLINNPHONGREFLECTIONMESHSHADER_H
#define ULTRENDERER_BLINNPHONGREFLECTIONMESHSHADER_H

#include "shaders/IMeshShader.h"

namespace UltRenderer {
    namespace Shaders {
        struct BlinnPhongReflectionMeshVarying: public IVarying {
            // TODO: Not support 3D texture for now
            Math::Vector3D uv;
        };

        class BlinnPhongReflectionMeshInterpolator: public IInterpolator<BlinnPhongReflectionMeshVarying> {
            BlinnPhongReflectionMeshVarying operator()(const std::array<BlinnPhongReflectionMeshVarying, 3>& varyings, const Math::Vector3D& weights) const override;
            BlinnPhongReflectionMeshVarying operator()(const std::array<BlinnPhongReflectionMeshVarying, 2>& varyings, const Math::Vector2D& weights) const override;
        };

        class BlinnPhongReflectionMeshVertexShader: public IMeshVertexShader<BlinnPhongReflectionMeshVarying> {
            BlinnPhongReflectionMeshVarying operator()(std::size_t vIdx) const override;
        };

        class BlinnPhongReflectionMeshFragmentShader: public IMeshFragmentShader<BlinnPhongReflectionMeshVarying> {
        public:
            bool operator()(const BlinnPhongReflectionMeshVarying& varying, Math::Vector4D& color, double& depth) const override;
        };
    } // Shaders
} // UltRenderer

#endif //ULTRENDERER_BLINNPHONGREFLECTIONMESHSHADER_H
