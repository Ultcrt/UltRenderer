//
// Created by ultcrt on 24-1-10.
//

#ifndef ULTRENDERER_NORMALMAPPINGMESHSHADER_H
#define ULTRENDERER_NORMALMAPPINGMESHSHADER_H

#include "IMeshShader.h"

namespace UltRenderer {
    namespace Rendering {
        namespace Rasterizing {
            namespace Shaders {
                class NormalMappingMeshInterpolator: public IInterpolator<IMeshVarying> {
                    IMeshVarying operator()(const std::array<IMeshVarying, 3>& varyings, const Math::Vector3D& weights) const override;
                    IMeshVarying operator()(const std::array<IMeshVarying, 2>& varyings, const Math::Vector2D& weights) const override;
                };

                class NormalMappingMeshVertexShader: public IMeshVertexShader<IMeshVarying> {
                    IMeshVarying operator()(std::size_t vIdx, Math::Vector4D& position) const override;
                };

                class NormalMappingMeshFragmentShader: public IMeshFragmentShader<IMeshVarying> {
                public:
                    bool operator()(const IMeshVarying& varying, const Math::Vector4D& fragCoord, Math::Vector4D& color, double& depth) const override;
                };
            } // Shaders
        } // Rasterizing
    } // Rendering

} // UltRenderer

#endif //ULTRENDERER_NORMALMAPPINGMESHSHADER_H
