//
// Created by ultcrt on 24-1-21.
//

#ifndef ULTRENDERER_DEPTHPEELINGMESHSHADER_H
#define ULTRENDERER_DEPTHPEELINGMESHSHADER_H

#include "IMeshShader.h"

namespace UltRenderer {
    namespace Rendering {
        namespace Rasterizing {
            namespace Shaders {
                class DepthPeelingMeshFragmentShader: public IMeshFragmentShader<IMeshVarying> {
                public:
                    const Data::Image* pLastDepthLayer;

                    bool operator()(const IMeshVarying& varying, const Math::Vector4D& fragCoord, Math::Vector4D& color, double& depth) const override;
                };
            } // Shaders
        } // Rasterizing
    } // Rendering
} // UltRenderer

#endif //ULTRENDERER_DEPTHPEELINGMESHSHADER_H
