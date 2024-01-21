//
// Created by ultcrt on 24-1-7.
//

#ifndef ULTRENDERER_FLATMESHSHADER_H
#define ULTRENDERER_FLATMESHSHADER_H

#include "shaders/IMeshShader.h"

namespace UltRenderer {
    namespace Shaders {
        class FlatMeshInterpolator: public IInterpolator<IMeshVarying> {
        public:
            IMeshVarying operator()(const std::array<IMeshVarying, 3>& varyings, const Math::Vector3D& weights) const override;
            IMeshVarying operator()(const std::array<IMeshVarying, 2>& varyings, const Math::Vector2D& weights) const override;
        };

        class FlatMeshVertexShader: public IMeshVertexShader<IMeshVarying> {
        public:
            IMeshVarying operator()(std::size_t vIdx, Math::Vector4D& position) const override;
        };

        class FlatMeshFragmentShader: public IMeshFragmentShader<IMeshVarying> {
        public:
            bool operator()(const IMeshVarying& varying, const Math::Vector4D& fragCoord, Math::Vector4D& color, double& depth) const override;
        };
    } // Shaders
} // UltRenderer

#endif //ULTRENDERER_FLATMESHSHADER_H
