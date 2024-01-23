//
// Created by ultcrt on 24-1-16.
//

#ifndef ULTRENDERER_PREBAKEDAMBIENTOCCLUSION_H
#define ULTRENDERER_PREBAKEDAMBIENTOCCLUSION_H

#include "data/Image.h"
#include "data/TriangleMesh.h"
#include "rendering/rasterizing/Camera.h"

namespace UltRenderer {
    namespace Rendering {
        namespace Rasterizing {
            namespace Bakers {
                class PreBakedAmbientOcclusion {
                public:
                    std::size_t samplingNum;
                    std::size_t width;
                    std::size_t height;

                    Data::Image operator()(const Data::TriangleMesh& mesh) const;
                };
            } // Bakers
        } // Rasterizing
    } // Rendering
} // UltRenderer

#endif //ULTRENDERER_PREBAKEDAMBIENTOCCLUSION_H
