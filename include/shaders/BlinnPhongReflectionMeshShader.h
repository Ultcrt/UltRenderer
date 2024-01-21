//
// Created by ultcrt on 24-1-10.
//

#ifndef ULTRENDERER_BLINNPHONGREFLECTIONMESHSHADER_H
#define ULTRENDERER_BLINNPHONGREFLECTIONMESHSHADER_H

#include "shaders/IMeshShader.h"

namespace UltRenderer {
    namespace Shaders {
        class BlinnPhongReflectionMeshInterpolator: public IInterpolator<IMeshVarying> {
            IMeshVarying operator()(const std::array<IMeshVarying, 3>& varyings, const Math::Vector3D& weights) const override;
            IMeshVarying operator()(const std::array<IMeshVarying, 2>& varyings, const Math::Vector2D& weights) const override;
        };

        class BlinnPhongReflectionMeshVertexShader: public IMeshVertexShader<IMeshVarying> {
            IMeshVarying operator()(std::size_t vIdx, Math::Vector4D& position) const override;
        };

        class BlinnPhongReflectionMeshFragmentShader: public IMeshFragmentShader<IMeshVarying> {
        public:
            double diffuseCoefficient;
            double specularCoefficient;
            double ambientCoefficient;

            Math::Vector3D specularColor;
            Math::Vector3D ambientColor;

            bool operator()(const IMeshVarying& varying, const Math::Vector4D& fragCoord, Math::Vector4D& color, double& depth) const override;
        };
    } // Shaders
} // UltRenderer

#endif //ULTRENDERER_BLINNPHONGREFLECTIONMESHSHADER_H
