//
// Created by ultcrt on 24-1-8.
//

#ifndef ULTRENDERER_PHONGMESHSHADER_H
#define ULTRENDERER_PHONGMESHSHADER_H

#include "shaders/IMeshShader.h"

namespace UltRenderer {
    namespace Shaders {
        class PhongMeshInterpolator: public IInterpolator<IMeshVarying> {
        public:
            IMeshVarying operator()(const std::array<IMeshVarying, 3>& varyings, const Math::Vector3D& weights) const override;
            IMeshVarying operator()(const std::array<IMeshVarying, 2>& varyings, const Math::Vector2D& weights) const override;
        };

        class PhongMeshVertexShader: public IMeshVertexShader<IMeshVarying> {
        public:
            IMeshVarying operator()(std::size_t vIdx, Math::Vector4D& position) const override;
        };

        class PhongMeshFragmentShader: public IMeshFragmentShader<IMeshVarying> {
        public:
            bool operator()(const IMeshVarying& varying, const Math::Vector4D& fragCoord, Math::Vector4D& color, double& depth) const override;
        };
    } // Shaders
} // UltRenderer

#endif //ULTRENDERER_PHONGMESHSHADER_H
