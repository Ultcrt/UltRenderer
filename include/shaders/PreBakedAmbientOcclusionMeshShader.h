//
// Created by ultcrt on 24-1-16.
//

#ifndef ULTRENDERER_PREBAKEDAMBIENTOCCLUSIONMESHSHADER_H
#define ULTRENDERER_PREBAKEDAMBIENTOCCLUSIONMESHSHADER_H

#include "shaders/IMeshShader.h"

namespace UltRenderer {
    namespace Shaders {
        class PreBakedAmbientOcclusionMeshFragmentShader: public IMeshFragmentShader<IMeshVarying> {
        private:
            Data::Image& _bakedTexture;

        public:
            explicit PreBakedAmbientOcclusionMeshFragmentShader(Data::Image& bakedTexture);

            bool operator()(const IMeshVarying& varying, Math::Vector4D& color, double& depth) const override;
        };

    } // Shaders
} // UltRenderer

#endif //ULTRENDERER_PREBAKEDAMBIENTOCCLUSIONMESHSHADER_H
