//
// Created by ultcrt on 24-1-10.
//

#ifndef ULTRENDERER_DEPTHMESHSHADER_H
#define ULTRENDERER_DEPTHMESHSHADER_H

#include "shaders/IMeshShader.h"

namespace UltRenderer {
    namespace Shaders {
        class DepthMeshInterpolator: public IInterpolator<IMeshVarying> {
            IMeshVarying operator()(const std::array<IMeshVarying, 3>& varyings, const Math::Vector3D& weights) const override;
            IMeshVarying operator()(const std::array<IMeshVarying, 2>& varyings, const Math::Vector2D& weights) const override;
        };

        class DepthMeshVertexShader: public IMeshVertexShader<IMeshVarying> {
            IMeshVarying operator()(std::size_t vIdx) const override;
        };

        class DepthMeshFragmentShader: public IMeshFragmentShader<IMeshVarying> {
        public:
            bool operator()(const IMeshVarying& varying, Math::Vector4D& color, double& depth) const override;
        };
    } // Shaders
} // UltRenderer

#endif //ULTRENDERER_DEPTHMESHSHADER_H
