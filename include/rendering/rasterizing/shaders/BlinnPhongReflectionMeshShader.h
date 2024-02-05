//
// Created by ultcrt on 24-1-10.
//

#ifndef ULTRENDERER_BLINNPHONGREFLECTIONMESHSHADER_H
#define ULTRENDERER_BLINNPHONGREFLECTIONMESHSHADER_H

#include "IMeshShader.h"

namespace UltRenderer {
    namespace Rendering {
        namespace Rasterizing {
            namespace Shaders {
                struct BlinnPhongReflectionMeshVarying: IMeshVarying {
                    Math::Vector3D viewPosition;
                };

                class BlinnPhongReflectionMeshInterpolator: public IInterpolator<BlinnPhongReflectionMeshVarying> {
                    BlinnPhongReflectionMeshVarying operator()(const std::array<BlinnPhongReflectionMeshVarying, 3>& varyings, const Math::Vector3D& weights) const override;
                    BlinnPhongReflectionMeshVarying operator()(const std::array<BlinnPhongReflectionMeshVarying, 2>& varyings, const Math::Vector2D& weights) const override;
                };

                class BlinnPhongReflectionMeshVertexShader: public IMeshVertexShader<BlinnPhongReflectionMeshVarying> {
                    BlinnPhongReflectionMeshVarying operator()(std::size_t vIdx, Math::Vector4D& position) const override;
                };

                class BlinnPhongReflectionMeshFragmentShader: public IMeshFragmentShader<BlinnPhongReflectionMeshVarying> {
                public:
                    bool operator()(const BlinnPhongReflectionMeshVarying& varying, const Math::Vector4D& fragCoord, Math::Vector4D& color, double& depth) const override;
                };
            } // Shaders
        } // Rasterizing
    } // Rendering
} // UltRenderer

#endif //ULTRENDERER_BLINNPHONGREFLECTIONMESHSHADER_H
