//
// Created by ultcrt on 24-1-7.
//

#ifndef ULTRENDERER_FLATMESHSHADER_H
#define ULTRENDERER_FLATMESHSHADER_H

#include "shaders/IMeshShader.h"

namespace UltRenderer {
    namespace Shaders {
        struct FlatMeshVarying: public IVarying {
            // TODO: Not support 3D texture for now
            Math::Vector3D uv;
            Math::Vector3D normal;
        };

        class FlatMeshInterpolator: public IInterpolator<FlatMeshVarying> {
        public:
            FlatMeshVarying operator()(const std::array<FlatMeshVarying, 3>& varyings, const Math::Vector3D& weights) const override;
            FlatMeshVarying operator()(const std::array<FlatMeshVarying, 2>& varyings, const Math::Vector2D& weights) const override;
        };

        class FlatMeshVertexShader: public IMeshVertexShader<FlatMeshVarying> {
        public:
            FlatMeshVarying operator()(std::size_t vIdx) const override;
        };

        class FlatMeshFragmentShader: public IMeshFragmentShader<FlatMeshVarying> {
        public:
            bool operator()(const FlatMeshVarying& varying, Math::Vector4D& color, double& depth) const override;
        };
    } // Shaders
} // UltRenderer

#endif //ULTRENDERER_FLATMESHSHADER_H
