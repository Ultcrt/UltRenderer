//
// Created by ultcrt on 24-1-10.
//

#ifndef ULTRENDERER_NORMALMAPPINGMESHSHADER_H
#define ULTRENDERER_NORMALMAPPINGMESHSHADER_H

#include "IMeshShader.h"

namespace UltRenderer {
    namespace Shaders {
        struct NormalMappingMeshVarying: public IVarying {
            // TODO: Not support 3D texture for now
            Math::Vector3D uv;
        };

        class NormalMappingMeshInterpolator: public IInterpolator<NormalMappingMeshVarying> {
            NormalMappingMeshVarying operator()(const std::array<NormalMappingMeshVarying, 3>& varyings, const Math::Vector3D& weights) const override;
            NormalMappingMeshVarying operator()(const std::array<NormalMappingMeshVarying, 2>& varyings, const Math::Vector2D& weights) const override;
        };

        class NormalMappingMeshVertexShader: public IMeshVertexShader<NormalMappingMeshVarying> {
            NormalMappingMeshVarying operator()(std::size_t vIdx) const override;
        };

        class NormalMappingMeshFragmentShader: public IMeshFragmentShader<NormalMappingMeshVarying> {
        public:
            bool operator()(const NormalMappingMeshVarying& varying, Math::Vector4D& color, double& depth) const override;
        };
    } // Shaders
} // UltRenderer

#endif //ULTRENDERER_NORMALMAPPINGMESHSHADER_H
