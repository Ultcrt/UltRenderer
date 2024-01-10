//
// Created by ultcrt on 24-1-8.
//

#ifndef ULTRENDERER_PHONGMESHSHADER_H
#define ULTRENDERER_PHONGMESHSHADER_H

#include "shaders/IMeshShader.h"

namespace UltRenderer {
    namespace Shaders {
        struct PhongMeshVarying: public IVarying {
            // TODO: Not support 3D texture for now
            Math::Vector3D uv;
            Math::Vector3D normal;
        };

        class PhongMeshInterpolator: public IInterpolator<PhongMeshVarying> {
        public:
            PhongMeshVarying operator()(const std::array<PhongMeshVarying, 3>& varyings, const Math::Vector3D& weights) const override;
            PhongMeshVarying operator()(const std::array<PhongMeshVarying, 2>& varyings, const Math::Vector2D& weights) const override;
        };

        class PhongMeshVertexShader: public IMeshVertexShader<PhongMeshVarying> {
        public:
            PhongMeshVarying operator()(std::size_t vIdx) const override;
        };

        class PhongMeshFragmentShader: public IMeshFragmentShader<PhongMeshVarying> {
        public:
            bool operator()(const PhongMeshVarying& varying, Math::Vector4D& color, double& depth) const override;
        };
    } // Shaders
} // UltRenderer

#endif //ULTRENDERER_PHONGMESHSHADER_H
