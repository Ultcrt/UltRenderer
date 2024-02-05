//
// Created by ultcrt on 24-1-16.
//

#ifndef ULTRENDERER_PREBAKEDAMBIENTOCCLUSIONMESHSHADER_H
#define ULTRENDERER_PREBAKEDAMBIENTOCCLUSIONMESHSHADER_H

#include "IMeshShader.h"

namespace UltRenderer {
    namespace Rendering {
        namespace Rasterizing {
            namespace Shaders {
                class PreBakedAmbientOcclusionMeshInterpolator: public IInterpolator<IMeshVarying> {
                    IMeshVarying operator()(const std::array<IMeshVarying, 3>& varyings, const Math::Vector3D& weights) const override;
                    IMeshVarying operator()(const std::array<IMeshVarying, 2>& varyings, const Math::Vector2D& weights) const override;
                };

                class PreBakedAmbientOcclusionMeshVertexShader: public IMeshVertexShader<IMeshVarying> {
                    IMeshVarying operator()(std::size_t vIdx, Math::Vector4D& position) const override;
                };

                class PreBakedAmbientOcclusionMeshFragmentShader: public IMeshFragmentShader<IMeshVarying> {
                private:
                    Data::Image& _bakedTexture;

                public:
                    explicit PreBakedAmbientOcclusionMeshFragmentShader(Data::Image& bakedTexture);

                    bool operator()(const IMeshVarying& varying, const Math::Vector4D& fragCoord, Math::Vector4D& color, double& depth) const override;
                };
            } // Shaders
        } // Rasterizing
    } // Rendering

} // UltRenderer

#endif //ULTRENDERER_PREBAKEDAMBIENTOCCLUSIONMESHSHADER_H
